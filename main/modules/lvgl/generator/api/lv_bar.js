module.exports = {
    // "lv_bar_create": [null, [], "lv_obj_t *"],
    "lv_bar_set_value": [null, [["int32_t","value"],["lv_anim_enable_t","anim"]], "void"],
    "lv_bar_set_start_value": [null, [["int32_t","start_value"],["lv_anim_enable_t","anim"]], "void"],
    "lv_bar_set_range": [null, [["int32_t","min"],["int32_t","max"]], "void"],
    "lv_bar_set_mode": [null, [["lv_bar_mode_t","mode"]], "void"],
    "lv_bar_get_value": [null, [], "int32_t"],
    "lv_bar_get_start_value": [null, [], "int32_t"],
    "lv_bar_get_min_value": [null, [], "int32_t"],
    "lv_bar_get_max_value": [null, [], "int32_t"],
    "lv_bar_get_mode": [null, [], "lv_bar_mode_t"],
}
