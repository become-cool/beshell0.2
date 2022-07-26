

## quickjs 连接错误:
```
1: undefined reference to `fesetround'
```


### 在 C代码里 判断 ArrayBuffer

直接取数据

```c
int length = 0 ;
uint8_t * buff = JS_GetArrayBuffer(ctx, &length, jsvar) ;
if(!buff || !length) {
    THROW_EXCEPTION("not array buffer")
}
```

在 c-js 函数中，可以使用自定义的宏 (utils.h)

```c
ARGV_TO_ARRAYBUFFER(0, buff, bufflen) 
```


## quickjs / c 对象混合编程

JSValue 是多种类型的联合体，按值传递，在 c 语言里记录 JSValue 的指针没有意义 (lvgl/moogoose 等库需要保存上下文数据的指针)，应该持有 JSValue 指向的 JSObject 指针。

使用 `JS_VALUE_GET_PTR(<JSValue>)` 宏可以取出 JSValue 指向的 JSObject 指针（如果该 JSValue 是object类型）；

使用 `JS_MKPTR(JS_TAG_OBJECT, <JSObject *>)` 宏将 JSObject 包装成一个 JSValue 传递给 js

> 当用变量是 `JSObject` 类型时， `JS_VALUE_GET_PTR` 和 `JS_MKPTR(JS_TAG_OBJECT, xxx)` 的作用是相反的

##  quickjs / c 内存管理

* `JS_NewObject` 创建一个 js 对象，确保有js变量指向这个对象，例如：可以 JS_SetPropertyStr(ctx, other, "xx", newobj) 设为其他对象的属性。否则 gc 无法自动回收，会造成内存邪路

* `JS_Eval` 返回值必须 JS_FreeValue ，否则可能内存泄漏
```
JSValue ret = JS_Eval(ctx, code, len, "filepath", JS_EVAL_TYPE_XXX)
JS_FreeValue(ctx, ret)
```
