* 编译过程

`idf.py set-target`

`idf.py dfu`

`idf.py dfu-flash`


* 接线方式

```
ESP32-S2 BOARD          USB CONNECTOR (type A)
                          --
                         | || VCC
    [GPIO 19]  --------> | || D-    白线
    [GPIO 20]  --------> | || D+    绿线
                         | || GND
                          --
```

* esp32-s2 GPIO19/20 usb 下载模式，需要 gpio0拉低时 reset， windows自动枚举到 esp32-s2 设备


* 执行 `dfu-util --list` 遇到错误

```
dfu-util 0.9

Copyright 2005-2009 Weston Schmidt, Harald Welte and OpenMoko Inc.
Copyright 2010-2016 Tormod Volden and Stefan Schmidt
This program is Free Software and has ABSOLUTELY NO WARRANTY
Please report bugs to http://sourceforge.net/p/dfu-util/tickets/

Cannot open DFU device 303a:0002
Cannot open DFU device 303a:0002
```

此时设备管理器里 esp32-s2 会被做为未知设备。

需要安装 usb通用驱动， windows 上可以下载 Zadig :
```
https://zadig.akeo.ie/
```

在 Zadig 中为已联机的 esp32-s2 设备安装 `WinUSB` 驱动。

然后设备管理器中 esp32-s2 被归做 `通用串行总线设备`
