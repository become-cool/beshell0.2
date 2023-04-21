

`beapi.lvgl.Style` 类是对 LVGL `lv_style_t` 的封装，一个 Style 对象可以通过`set(name, value)` 设置多个样式的属性值。



`beapi.lvgl.Obj` 和它的子类通过 `addStyle(style)` 添加 `Style` 对象。但是更简便的方式是使用 `Obj.setStyle(name,value)` 直接设置样式值。



## 方法

### set(name: string, value:any): void

设置样式的属性值



### get(name: string): any

返回样式的属性值



### props(): string[]

返回所有已经设置的属性名称



### setPadAll(pad:number): void

同时在所有方向上设置 pad ，请参考 lvgl `lv_style_set_pad_all`





## 属性值



${ALL STYLES}
