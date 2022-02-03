
### lvgl WASM API

> 需要 clang-9 (支持 json 输出) , `sudo apt install clang-9`

生成 lvgl ast json 文件：

```
clang-9 -cc1 -internal-isystem /usr/local/include -internal-isystem /usr/lib/llvm-9/lib/clang/9.0.0/include -i nternal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -I../main/modules/lvgl -ast-dump=json ../components/lvgl/src/lvgl.h > lvgl-ast.json
```

