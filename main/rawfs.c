#include "rawfs.h"

#include <unistd.h>
#include <dirent.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/lock.h>
#include <sys/param.h>
#include "esp_vfs.h"
#include "esp_partition.h"
#include "debug.h"
#include "untar.h"



#define MAX_OPEN_FD 16

extern const uint8_t rawfs_root_start[] asm("_binary_root_raw_start");
extern const uint8_t rawfs_root_end[] asm("_binary_root_raw_end");


#define VFS_NODE_DIR 1
#define VFS_NODE_FILE 2

typedef struct _vfs_node {

    unsigned char type ;
    char * filename ;
    uint32_t offset ;
    uint32_t filesize ;

    struct _vfs_node * next ;
    struct _vfs_node * children ;

} vfs_node_t ;


typedef struct {
    DIR dir;            /*!< VFS DIR struct */
    struct dirent e;    /*!< Last open dirent */

    vfs_node_t * node ;
    vfs_node_t * walk_child ;

} vfs_rawfs_dir_t;


typedef struct {
    vfs_node_t * node ;
    uint32_t offset ;
} vfs_rawfs_fd_t;


typedef struct _vfs_rawfs {
    vfs_node_t * root ;
    uint8_t * raw ;
    uint8_t * raw_data ;
    size_t * size ;

    vfs_rawfs_fd_t fds[MAX_OPEN_FD] ;

} vfs_rawfs_t ;

static vfs_node_t * vfs_node_malloc(vfs_node_t * parent) {  
    vfs_node_t * node = malloc( sizeof(vfs_node_t) ) ;
    memset(node, 0, sizeof(vfs_node_t) );

    if(parent) {
        node->next = parent->children ;
        parent->children = node ;
    }

    return node ;
}

static void free_node(vfs_node_t * node) {
    free(node) ;
}

static vfs_node_t * vfs_node_get_child_by_name(vfs_node_t * parent, const char * name) {
    for(vfs_node_t * child=parent->children; child!=NULL; child=child->next) {
        if(strcmp(child->filename, name) == 0) {
            return child;
        }
    }
    return NULL ;
}


static vfs_node_t * vfs_node_walk_path(vfs_node_t * parent, char * path) {
    if(path[0]=='/') {
        path ++ ;
    }
    // 末尾 / 
    if(strlen(path)==0) {
        return parent ;
    }

    char * p = strchr(path,'/') ;
    int namelen = 0 ;
    if(p) {
        namelen = p - path ;
    }
    else {
        namelen = strlen(path) ;
    }

    if( namelen==0 || (namelen==1 && path[0]=='.') ) {
        return vfs_node_walk_path(parent, p) ;
    }
    
    char e = path[namelen] ;
    path[namelen] = 0 ;

    vfs_node_t * node = vfs_node_get_child_by_name(parent, path) ;
    if(node==NULL) {
        return NULL ;
    }

    path[namelen] = e ;

    // 叶节点
    if(!p) {
        return node ;
    }
    if(*p=='/'){
        p ++ ;
    }
    if(strlen(p)<=0) {
        // over
        return node ;
    }

    // 递归
    return vfs_node_walk_path(node, p) ;
}


#define WARNING_READONLY(op) \
    printf("read only FS operation not supported: %s.\n",op) ;
#define RAWFS ((vfs_rawfs_t *)ctx)

static int vfs_rawfs_open(void* ctx, const char * path, int flags, int mode) {
    vfs_node_t * node = vfs_node_walk_path(RAWFS->root, path) ;
    if(!node) {
        return -1 ;
    }
    if( node->type != VFS_NODE_FILE ){
        return -2 ;
    }

    uint8_t fd = 0 ;
    for(;fd<MAX_OPEN_FD;fd++) {
        if( ! RAWFS->fds[fd].node) {
            break ;
        }
    }

    if(fd>=MAX_OPEN_FD) {
        printf("too much file opened: %d\n", MAX_OPEN_FD) ;
        return -3 ;
    }

    RAWFS->fds[fd].node = node ;
    RAWFS->fds[fd].offset = 0 ;

    return  fd ;
}
static ssize_t vfs_rawfs_write(void* ctx, int fd, const void * data, size_t size) {
    WARNING_READONLY("write")
    return -1 ;
}
static ssize_t vfs_rawfs_read(void* ctx, int fd, void * dst, size_t size) {
    if(fd<0 || fd>=MAX_OPEN_FD) {
        return -1 ;
    }
    if(!RAWFS->fds[fd].node) {
        return -2 ;
    }
    if(RAWFS->fds[fd].offset + size > RAWFS->fds[fd].node->filesize) {
        size = RAWFS->fds[fd].node->filesize - RAWFS->fds[fd].offset ;
    }

    memcpy(dst, RAWFS->raw_data+RAWFS->fds[fd].node->offset + RAWFS->fds[fd].offset, size) ;
    RAWFS->fds[fd].offset+= size ;

    return size ;
}
static ssize_t vfs_rawfs_pwrite(void *ctx, int fd, const void *src, size_t size, off_t offset) {
    WARNING_READONLY("pwrite")
    return -1 ;
}
static int vfs_rawfs_close(void* ctx, int fd) {
    if(fd<0 || fd>=MAX_OPEN_FD) {
        return -1 ;
    }
    RAWFS->fds[fd].node = NULL ;
    RAWFS->fds[fd].offset = 0 ;
    return 0 ;
}
static off_t vfs_rawfs_lseek(void* ctx, int fd, off_t offset, int mode) {
    if(fd<0 || fd>=MAX_OPEN_FD) {
        return -1 ;
    }
    if(!RAWFS->fds[fd].node) {
        return -2 ;
    }
    if( offset > RAWFS->fds[fd].node->filesize ) {
        return -3 ;
    }

    RAWFS->fds[fd].offset = offset ;
    return 0 ;
}
static ssize_t vfs_rawfs_pread(void *ctx, int fd, void *dst, size_t size, off_t offset) {
    vfs_rawfs_lseek(ctx, fd, offset, 0) ;
    return vfs_rawfs_read(ctx,fd,dst,size) ;
}
static int vfs_rawfs_stat(void* ctx, const char * path, struct stat * st) {
    vfs_node_t * node = vfs_node_walk_path(RAWFS->root, path) ;
    if(!node) {
        return -1 ;
    }

	st->st_size = node->filesize ;
    st->st_mode = ((node->type==VFS_NODE_FILE)?S_IFREG:S_IFDIR);
    st->st_atime = 0;
    st->st_mtime = 0;
    st->st_ctime = 0;
    st->st_dev = 0;
    st->st_ino = 0;
    
    return 0 ;
}
static int vfs_rawfs_unlink(void* ctx, const char *path) {
    WARNING_READONLY("unlink")
    return -1 ;
}
static int vfs_rawfs_rename(void* ctx, const char *src, const char *dst) {
    WARNING_READONLY("rename")
    return -1 ;
}
static DIR* vfs_rawfs_opendir(void* ctx, const char* name) {
    vfs_node_t * dirnode = vfs_node_walk_path(RAWFS->root, name) ;
    if(!dirnode) {
        return NULL ;
    }

    vfs_rawfs_dir_t * dir = malloc(sizeof(vfs_rawfs_dir_t)) ;
    memset(dir, 0, sizeof(vfs_rawfs_dir_t)) ;
    dir->node = dirnode ;
    dir->walk_child = dirnode->children ;

    return (DIR*)dir ;
}
static int vfs_rawfs_closedir(void* ctx, DIR* pdir) {
    if(!pdir) {
        return  0 ;
    }
    free(pdir) ;
    return 0 ;
}
static int vfs_rawfs_readdir_r(void* ctx, DIR* pdir, struct dirent* entry, struct dirent** out_dirent) { 
    vfs_rawfs_dir_t * dir = (vfs_rawfs_dir_t*) pdir ;
    if(!dir->walk_child) {
        *out_dirent = NULL ;
        return 0 ;
    }
    
    strcpy(entry->d_name, dir->walk_child->filename) ;
    entry->d_type = dir->walk_child->type ;

    (*out_dirent) = entry ;
    
    dir->walk_child = dir->walk_child->next ;

    return 0 ;
}
static struct dirent* vfs_rawfs_readdir(void* ctx, DIR* pdir) {
    if(!pdir) {
        return  0 ;
    }
    struct dirent * out_dirent ;
    if(vfs_rawfs_readdir_r(RAWFS->root, pdir, &(((vfs_rawfs_dir_t*) pdir)->e), &out_dirent)!=0) {
        return NULL ;
    }

    return out_dirent ;
}
static long vfs_rawfs_telldir(void* ctx, DIR* pdir) {
    WARNING_READONLY("telldir")
    return 0 ;
}
static void vfs_rawfs_seekdir(void* ctx, DIR* pdir, long offset) {
    WARNING_READONLY("seekdir")
    return ;
}
static int vfs_rawfs_mkdir(void* ctx, const char* name, mode_t mode) {
    WARNING_READONLY("mkdir")
    return 0 ;
}
static int vfs_rawfs_rmdir(void* ctx, const char* name) {
    WARNING_READONLY("rmdir")
    return 0 ;
}
static int vfs_rawfs_fsync(void* ctx, int fd) {
    WARNING_READONLY("fsync")
    return 0 ;
}

vfs_node_t * parse_tree(char * raw, vfs_node_t * parent, char ** out_raw) {
    
    vfs_node_t * node = vfs_node_malloc(parent) ;
    if(!node) {
        return NULL ;
    }

    node->type = *raw ;
    raw ++ ;

    node->filename = raw ;
    raw+= strlen(node->filename) + 1 ;

    if(node->type==VFS_NODE_FILE) {
        node->offset = * ((uint32_t *)raw) ;
        raw+= sizeof(uint32_t) ;
    }

    node->filesize = * ((uint32_t *)raw) ;
    raw+= sizeof(uint32_t) ;

    if(node->type==VFS_NODE_DIR) {
        for(int i=0;i<node->filesize;i++) {
            parse_tree(raw, node, &raw) ;
        }
    }

    if(out_raw) {
        *out_raw = raw ;
    }

    return node ;
}

void be_rawfs_mount(const char * mntPoint) {

    vfs_rawfs_t * rawfs = malloc( sizeof(vfs_rawfs_t) ) ;
    rawfs->root = parse_tree(rawfs_root_start, NULL, &rawfs->raw_data) ;
    rawfs->raw = rawfs_root_start ;
    rawfs->size = rawfs_root_end - rawfs_root_start ;

    memset(rawfs->fds, 0, sizeof(vfs_rawfs_fd_t)*MAX_OPEN_FD) ;

    const esp_vfs_t vfs = {
        .flags       = ESP_VFS_FLAG_CONTEXT_PTR,
        .write_p     = &vfs_rawfs_write,
        .pwrite_p    = &vfs_rawfs_pwrite,
        .lseek_p     = &vfs_rawfs_lseek,
        .read_p      = &vfs_rawfs_read,
        .pread_p     = &vfs_rawfs_pread,
        .open_p      = &vfs_rawfs_open,
        .close_p     = &vfs_rawfs_close,
        // .fstat_p     = &vfs_rawfs_fstat,
        .fstat_p     = NULL,
        .stat_p      = &vfs_rawfs_stat,
        .link_p      = NULL, /* Not Supported */
        .unlink_p    = &vfs_rawfs_unlink,
        .rename_p    = &vfs_rawfs_rename,
        .opendir_p   = &vfs_rawfs_opendir,
        .closedir_p  = &vfs_rawfs_closedir,
        .readdir_p   = &vfs_rawfs_readdir,
        .readdir_r_p = &vfs_rawfs_readdir_r,
        .seekdir_p   = &vfs_rawfs_seekdir,
        .telldir_p   = &vfs_rawfs_telldir,
        .mkdir_p     = &vfs_rawfs_mkdir,
        .rmdir_p     = &vfs_rawfs_rmdir,
        .fsync_p     = &vfs_rawfs_fsync,
        // .utime_p     = &vfs_rawfs_utime,
        .utime_p     = NULL,
    };

    esp_err_t err = esp_vfs_register(mntPoint, &vfs, rawfs);
    if (err != ESP_OK) {
        printf("Failed to register tar fs");
        return ;
    }

    // untar
    // read_tar_from_flash(rawfs) ;

    return ;
}

// esp_partition_read
