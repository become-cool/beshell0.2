# 二次开发内容

typedef struct {
    lv_obj_t obj;
    char ** map;
    uint16_t tab_cnt;
    uint16_t tab_cur;
    lv_dir_t tab_pos;
    lv_anim_enable_t anim ;
} lv_tabview_t;

* `lv_tabview_t` 增加 `anim`  字段

```
typedef struct {
    lv_obj_t obj;
    char ** map;
    uint16_t tab_cnt;
    uint16_t tab_cur;
    lv_dir_t tab_pos;
    lv_anim_enable_t anim ;  // 增加
} lv_tabview_t;
```

* 增加 `lv_tabview_set_anim()` api 函数
* 增加 `lv_tabview_get_anim()` api 函数

