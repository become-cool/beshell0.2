## 0.2.14

* fixed: beapi.fs.info() 效率很低的问题

* 完全重构了 i2c 输入设备驱动的代码: 

    * 采用队列的方式在两个cpu之前传递消息

    * 所有 i2c 输入设备, 集中在通一个任务循环里读取

    * 不再被动等待 lvgl timer 读取i2c输入事件, 将事件主动投递给 lvgl

## 0.2.13 2023-04-20

增加:

* beapi.lvgl.InDevNav 类支持模拟输入方式

* 增加 beapi.lvgl.allIndev() api 函数

* 增加 beapi.lvgl.findIndevById() api 函数
