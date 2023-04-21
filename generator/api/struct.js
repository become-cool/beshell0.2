const LstStuct = [
    {
        name: 'draw_rect_dsc' ,
        type: 'lv_draw_rect_dsc_t' ,
        jsclass: 'DrawRectDsc' ,
        members: [
            ["lv_coord_t", "radius"] ,
            // ["lv_blend_mode_t", "blend_mode"] ,
            /*Background*/
            ["lv_color_t", "bg_color"] ,
            ["lv_color_t", "bg_grad_color"] ,
            ["uint8_t", "bg_main_color_stop"] ,
            ["uint8_t", "bg_grad_color_stop"] ,
            ["lv_opa_t", "bg_opa"] ,
            // ["lv_grad_dir_t", "bg_grad_dir"] ,
            /*Background img*/
            // ["const void *", "bg_img_src"] ,
            // ["const void *", "bg_img_symbol_font"] ,
            ["lv_color_t", "bg_img_recolor"] ,
            ["lv_opa_t", "bg_img_opa"] ,
            ["lv_opa_t", "bg_img_recolor_opa"] ,
            ["uint8_t", "bg_img_tiled"] ,
            /*Border*/
            ["lv_color_t", "border_color"] ,
            ["lv_coord_t", "border_width"] ,
            ["lv_opa_t", "border_opa"] ,
            ["uint8_t", "border_post"] ,
            ["lv_border_side_t", "border_side"] ,
            /*Outline*/
            ["lv_color_t", "outline_color"] ,
            ["lv_coord_t", "outline_width"] ,
            ["lv_coord_t", "outline_pad"] ,
            ["lv_opa_t", "outline_opa"] ,
            /*Shadow*/
            ["lv_color_t", "shadow_color"] ,
            ["lv_coord_t", "shadow_width"] ,
            ["lv_coord_t", "shadow_ofs_x"] ,
            ["lv_coord_t", "shadow_ofs_y"] ,
            ["lv_coord_t", "shadow_spread"] ,
            ["lv_opa_t", "shadow_opa"] ,
        ] ,
        init: 'lv_draw_rect_dsc_init'
    } ,

    {
        name: 'draw_arc_dsc' ,
        type: 'lv_draw_arc_dsc_t' ,
        jsclass: 'DrawArcDsc' ,
        members: [
            ['lv_color_t', 'color'] ,
            ['lv_coord_t', 'width'] ,
            // ['const void *', 'img_src'] ,
            ['lv_opa_t', 'opa'] ,
            // ['lv_blend_mode_t', 'blend_mode'] ,
            ['uint8_t', 'rounded'] ,
        ] ,
        init: 'lv_draw_arc_dsc_init'
    } ,
    
    {
        name: 'draw_line_dsc' ,
        type: 'lv_draw_line_dsc_t' ,
        jsclass: 'DrawLineDsc' ,
        members: [
            ['lv_color_t', 'color'] ,
            ['lv_coord_t', 'width'] ,
            ['lv_coord_t', 'dash_width'] ,
            ['lv_coord_t', 'dash_gap'] ,
            // ['const void *', 'img_src'] ,
            ['lv_opa_t', 'opa'] ,
            // ['lv_blend_mode_t', 'blend_mode'] ,
            ['uint8_t', 'round_start'] ,
            ['uint8_t', 'round_end'] ,
            ['uint8_t', 'raw_end'] ,
        ] ,
        init: 'lv_draw_line_dsc_init'
    } ,
    
    {
        name: 'area' ,
        type: 'lv_area_t' ,
        jsclass: 'Area' ,
        args: [
            ['lv_coord_t', 'x1'] ,
            ['lv_coord_t', 'y1'] ,
            ['lv_coord_t', 'x2'] ,
            ['lv_coord_t', 'y2'] ,
        ] ,
        members: [
            ['lv_coord_t', 'x1'] ,
            ['lv_coord_t', 'y1'] ,
            ['lv_coord_t', 'x2'] ,
            ['lv_coord_t', 'y2'] ,
        ] ,
        // init: 'memset'
    } ,


    // mask
    // {
    //     name: 'draw_mask_radius_param' ,
    //     type: 'lv_draw_mask_radius_param_t' ,
    //     jsclass: 'DrawMaskRadius' ,
    //     members: [
    //         ['lv_coord_t', 'x1'] ,
    //         ['lv_coord_t', 'y1'] ,
    //         ['lv_coord_t', 'x2'] ,
    //         ['lv_coord_t', 'y2'] ,
    //     ] ,
    //     init: 'lv_draw_mask_radius_init'
    // } ,

    // 

    
]

LstStuct.byName = {}
for(let def of LstStuct) {
    LstStuct.byName[def.type] = def
}

module.exports = LstStuct