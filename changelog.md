
## 0.2.15

* 监听 80 端口的脚本放在 ~/www 目录内，可以由用户改写

* beapi.mg.httpListen 返回的对象只有1个引用，没有赋值给变量会立即释放

* wsam 版本可用

* fixed: repl 下 require 错误不显示 stack

* 增加了 WS2812 驱动

* cron

* fixed: uart telnet 协议发送数据快的时候会出错的问题

## 0.2.14 2023-104-28

* fixed: beapi.fs.info() 效率很低的问题

* 完全重构了 i2c 输入设备驱动的代码: 

    * 采用队列的方式在两个cpu之前传递消息

    * 所有 i2c 输入设备, 集中在通一个任务循环里读取

    * 不再被动等待 lvgl timer 读取i2c输入事件, 将事件主动投递给 lvgl

* 重写了 beapi.gpio.pinMode() 的实现

* 增加了位移寄存器(74HC165)的驱动 beapi.driver.common.shift_read()

* http fs 接口支持跨域访问


## 0.2.13 2023-04-20

增加:

* beapi.lvgl.InDevNav 类支持模拟输入方式

* 增加 beapi.lvgl.allIndev() api 函数

* 增加 beapi.lvgl.findIndevById() api 函数
