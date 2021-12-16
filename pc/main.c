#include "utils.h"
#include "module_fs.h"
#include "task_js.h"
#include "anchor9.h"

void main( int argc, char *argv[] ){

    if(argc>1) {
        module_fs_set_vfs_path_prefix(argv[1]) ;
    }
    else {
        module_fs_set_vfs_path_prefix("../filesystem/root/") ;
    }

    task_js_main() ;
}
