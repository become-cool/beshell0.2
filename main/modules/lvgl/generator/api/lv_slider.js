module.exports = {
    // "lv_slider_create": [null, [], "lv_obj_t *"],
    "lv_slider_set_value": [null, [["int32_t","value"],["lv_anim_enable_t","anim", "true"]], "void"],
    "lv_slider_set_left_value": [null, [["int32_t","value"],["lv_anim_enable_t","anim", "true"]], "void"],
    "lv_slider_set_range": [null, [["int32_t","min"],["int32_t","max"]], "void"],
    "lv_slider_set_mode": [null, [["lv_slider_mode_t","mode"]], "void"],
    "lv_slider_get_value": [null, [], "int32_t"],
    "lv_slider_get_left_value": [null, [], "int32_t"],
    "lv_slider_get_min_value": [null, [], "int32_t"],
    "lv_slider_get_max_value": [null, [], "int32_t"],
    "lv_slider_is_dragged": [null, [], "bool"],
    "lv_slider_get_mode": [null, [], "lv_slider_mode_t"],
}
