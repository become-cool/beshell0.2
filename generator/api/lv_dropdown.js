module.exports = {
    // "lv_dropdown_create": [null, [], "lv_obj_t *"],
    "lv_dropdown_set_text": [null, [["char *","txt"]], "void"],
    "lv_dropdown_set_options": [null, [["char *","options"]], "void"],
    "lv_dropdown_set_options_static": [null, [["char *","options"]], "void"],
    "lv_dropdown_add_option": [null, [["char *","option"],["uint32_t","pos"]], "void"],
    "lv_dropdown_clear_options": [null, [], "void"],
    "lv_dropdown_set_selected": [null, [["uint16_t","sel_opt"]], "void"],
    "lv_dropdown_set_dir": [null, [["lv_dir_t","dir"]], "void"],
    // "lv_dropdown_set_symbol": [null, [["void *","symbol"]], "void"],
    "lv_dropdown_set_selected_highlight": [null, [["bool","en"]], "void"],
    // "lv_dropdown_get_list": [null, [], "lv_obj_t *"],
    "lv_dropdown_get_text": [null, [], "char *"],
    "lv_dropdown_get_options": [null, [], "char *"],
    "lv_dropdown_get_selected": [null, [], "uint16_t"],
    "lv_dropdown_get_option_cnt": [null, [], "uint16_t"],
    "lv_dropdown_get_selected_str": [null, [["char *","buf"],["uint32_t","buf_size"]], "void"],
    "lv_dropdown_get_symbol": [null, [], "char *"],
    "lv_dropdown_get_selected_highlight": [null, [], "bool"],
    "lv_dropdown_get_dir": [null, [], "lv_dir_t"],
    "lv_dropdown_open": [null, [], "void"],
    "lv_dropdown_close": [null, [], "void"],
}