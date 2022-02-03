module.exports = {
    // "lv_roller_create": [null, [], "lv_obj_t *"],
    "lv_roller_set_options": [null, [["char *","options"],["lv_roller_mode_t","mode"]], "void"],
    "lv_roller_set_selected": [null, [["uint16_t","sel_opt"],["lv_anim_enable_t","anim"]], "void"],
    "lv_roller_set_visible_row_count": [null, [["uint8_t","row_cnt"]], "void"],
    "lv_roller_get_selected": [null, [], "uint16_t"],
    "lv_roller_get_selected_str": [null, [["char *","buf"],["uint32_t","buf_size"]], "void"],
    "lv_roller_get_options": [null, [], "char *"],
    "lv_roller_get_option_cnt": [null, [], "uint16_t"],
}
