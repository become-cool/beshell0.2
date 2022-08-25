
### lvgl WASM API

> 需要 clang-9 (支持 json 输出) , `sudo apt install clang-9`

生成 lvgl ast json 文件：

```
clang-9 -cc1 -internal-isystem /usr/local/include -internal-isystem /usr/lib/llvm-9/lib/clang/9.0.0/include -i nternal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -I../main/modules/lvgl -ast-dump=json ../components/lvgl/src/lvgl.h > lvgl-ast.json
```

## 脚本说明


### scan_api.js

扫描分析 lvgl C源代码，将指定widget前缀的 api 函数输出为 js(json) 格式，用于后续 js 代码生成，保存在 api 目录内。

然后需要在 `lv_widgets.meta.js` 中 require 该文件

例如： 
```
node scan_api.js lv_obj > api/lv_obj.js
```


### generate-widget.c.js

自动生成文件： 
* ../main/modules/lvgl/widgets_gen.h  
* ../main/modules/lvgl/widgets_gen.c  
* ../main/modules/lvgl/be_lv_struct_wrapper.c


