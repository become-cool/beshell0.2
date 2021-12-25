module.exports = {
    // "lv_list_create": [null, [], "lv_obj_t *"],
    "lv_list_add_text": ["addText", [["char *","txt"]], "lv_obj_t *"],
    "lv_list_add_btn": ["addBtn", [["char *","icon"],["char *","txt"]], "lv_obj_t *"],
    "lv_list_get_btn_text": ["getBtnText", [["lv_obj_t *","btn"]], "char *"],
}
