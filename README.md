## 编译

```
source esp-idf/export.sh
./make
```

## quickjs 连接错误:
```
1: undefined reference to `fesetround'
```


## quickjs / c 对象混合编程

JSValue 是多种类型的联合体，按值传递，在 c 语言里记录 JSValue 的指针没有意义 (lvgl/moogoose 等库需要保存上下文数据的指针)，应该持有 JSValue 指向的 JSObject 指针。

通过 `JS_VALUE_GET_PTR(<JSValue>)` 宏可以取出 JSValue 指向的 JSObject 指针（如果该 JSValue 是object类型）；

通过 `JS_MKPTR(JS_TAG_OBJECT, <JSObject *>)` 宏将 JSObject 包装成一个 JSValue 传递给 js


##  quickjs / c 内存管理

* `JS_NewObject` 创建一个 js 对象，确保有js变量指向这个对象，例如：可以 JS_SetPropertyStr(ctx, other, "xx", newobj) 设为其他对象的属性。否则 gc 无法自动回收，会造成内存邪路

* `JS_Eval` 返回值必须 JS_FreeValue ，否则可能内存泄漏
```
JSValue ret = JS_Eval(ctx, code, len, "filepath", JS_EVAL_TYPE_XXX)
JS_FreeValue(ctx, ret)
```



## GPIO12 启动时上拉状态

在 ESP32 中，GPIO12 是硬件 SPI 的 MISO MUX引脚, SPI 使用 MUX 引脚可高达 80MHz 频率，其它任意引进仅为 26MHz。
对于LCD串口屏幕来说，需要高频率SPI，以便达到较高帧率。

作为 SPI/I2C 等串口引脚，往往需要上拉，以在较高频率下，获得足够的电流驱动能力。有些芯片甚至会在内部将 SDA、MISO 等引脚设置为高阻态，这相当于上拉。

但是 GPIO12 同时也是 ESP32 MTDI , 启动时， MTDI 高/低电平，分别标识 flsah 电压 1.8V/3.3V 。

ESP32在启动时，如果遇到 GPIO12 上拉，则 flash 在 1.8V 工作；而外置的 flash 芯片为 3.3V 工作电压，则 flash 没有启动， ESP32 会触发看门狗错误，无限循环重启：

```
rst:0x10 (RTCWDT_RTC_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
```

解决方案：

* 使用 1.8V flash

* 修改 efuses 配置，固定以 3.3V 驱动flash，不再检查 GPIO12 ：

    ```
    python esp-idf/components/esptool_py/esptool/espefuse.py set_flash_voltage 3.3V --port COM9
    ```

官方说明：
```
https://github.com/espressif/esp-idf/tree/master/examples/storage/sd_card#note-about-gpio12-esp32-only
```

efuses 是一次性编程，不能重复修改

## 无限重启

esp32 启动时电流较大，sgm2019 很容易支持不了，导致无限重启，可以在 3V3 和 GND 之间加一个大电容 (47uF)


## esp32 程序较大时，上电启动慢，reset/reboot 不受影响

esp32芯片串口上的启动输出显示 启动了两次，第一次中途僵死，等待几秒后 reset 。

可能是 bootloader.bin 在启动程序时没有及时“喂狗”，导致 watchdog 重置系统。

在 `esp-idf/components/esp32/cpu_start.c` 文件的 `call_start_cpu0()` 函数内， `ESP_EARLY_LOGI(TAG, "Starting app cpu, entry point is %p", call_start_cpu1);` 
附近输出一些文本内容，重新编译 bootloader.bin 可解决（未充分证实）。


> 编译 bootloader.bin 方法：复制 `esp-idf/components/bootloader` 到项目的 `components` 目录下，然后重新编译项目


## 分区

* 根据 cvs 生成分区文件：

```
cd filesystem
make
```

* 分区文件转换为 CSV 文件:
```
python gen_esp32part.py binary_partitions.bin input_partitions.csv
```


## eFuse

工具文档：

```
https://github.com/espressif/esptool/blob/master/docs/en/espefuse/index.rst
```

可在出厂时将产品型号写入 efuse ，在 js 运行时读取

> esp_idf c++ 读取 efuse :
>
> API: https://github.com/espressif/esp-idf/tree/master/components/efuse
>
> 例子 : https://github.com/espressif/esp-idf/blob/master/examples/system/efuse


烧写命令:
```
python <esp-idf path>/components/esptool_py/esptool/espefuse.py --port COM18 burn_block_data BLOCK3 <bin_file_path>
```

## esp-idf SD 限制

esp-idf 默认不支持 exFAT ，FAT32最大容量为 32GB, 大于 32GB windows 上格式化时只有 exFAT 选项。


esp-idf/components/fatfs/src/ffconf.h 文件中：
```
#define FF_FS_EXFAT		0
```

同时需要再 sdkconfig 文件 (或 idf menuconf) 中打开 LFN (LFN>=1)
```
CONFIG_FATFS_LFN_STACK=y
CONFIG_FATFS_API_ENCODING_UTF_8=y
```

## S2 摄像头驱动问题

配置摄像头的时候 `camera_config_t.clk_freq_hz` 必须设置为 16MHz , 否则驱动在分配 DMA 内存时，不会使用 PSRAM ，导致内存紧张。如果再启用 WiFi ，则 SRAM 不够用

```
static camera_config_t camera_config = {

    ...

    //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
    // .xclk_freq_hz = 20000000,
    .xclk_freq_hz = 16000000,
    
    ...
}
```


## error: 'rtc_gpio_desc' undeclared

这是从 arduino 的 esp32 库里拷贝来的代码， 4.2 以后的 esp-idf 需要在  `idf.py menuconfig` 中配置 一下：

```
Component config > driver configurations > RTCI0
```

## WiFi 问题

`wifi` 会占用很大 DMA 内存, 如果事先占用过多 DMA 内存, 则在 `esp_wifi_start()` 时会直接崩溃, 或启动 AP 后拒绝其他设备连接.

`heap_caps_malloc(N, MALLOC_CAP_DMA)` 函数调用会分配 DMA 内存.

出于性能考虑, IO密集型任务, 往往也需要 DMA内存, 需要额外注意 DMA内存的使用.

例如 `lvgl`的显存 (lv_disp_draw_buf_init) , 经过反复实验, 不要超过 `320*15*2` .


## 程序崩溃时调用路径定位

> 需要在 menuconfig 种设置串口打印crash信息

崩溃时串口上输出crash信息, 例如：

```
Backtrace:0x4008f4c2:0x3ffc3420 0x4008fccd:0x3ffc3440 0x40097a36:0x3ffc3460 0x4017acc3:0x3ffc34d0 0x4019ac39:0x3ffc34f0 0x400deb7b:0x3ffc3530 0x400f92d2:0x3ffc3580 0x400e8c79:0x3ffc35d0 0x400dd9e1:0x3ffc3610 0x400dda15:0x3ffc3640 0x400ddb0e:0x3ffc3670 0x4009337e:0x3ffc3690
```

复制 `Backtrace:` 后的内容，在 beshell 项目目录内执行：
```
xtensa-esp32-elf-addr2line -pfiaC -e build/beshell.elf 0x401b3e2f:0x3ffcbc30 0x400e95f3:0x3ffcbc50 0x400e9ae1:0x3ffcbc70 0x400eacf5:0x3ffcbcb0 0x400deabf:0x3ffcbcd0 0x400934ee:0x3ffcbcf0
```

输出：
```
0x4008f4c2: panic_abort at /mnt/d/lib/esp-idf/components/esp_system/panic.c:356
0x4008fccd: esp_system_abort at /mnt/d/lib/esp-idf/components/esp_system/system_api.c:112
0x40097a36: abort at /mnt/d/lib/esp-idf/components/newlib/abort.c:46
0x4017acc3: JS_DefineAutoInitProperty at /mnt/d/project/robot/beshell/build/../components/quickjs/quickjs.c:9234
0x4019ac39: JS_SetPropertyFunctionList at /mnt/d/project/robot/beshell/build/../components/quickjs/quickjs.c:35770
0x400deb7b: qjs_def_class at /mnt/d/project/robot/beshell/build/../main/utils.c:100
0x400f92d2: be_lv_widgets_gen_require at /mnt/d/project/robot/beshell/build/../main/modules/lvgl/widgets_gen.c:7467
0x400e8c79: be_module_lvgl_require at /mnt/d/project/robot/beshell/build/../main/modules/lvgl/module_lvgl.c:328
0x400dd9e1: JS_NewCustomContext at /mnt/d/project/robot/beshell/build/../main/task_js.c:141
0x400dda15: quickjs_init at /mnt/d/project/robot/beshell/build/../main/task_js.c:153
0x400ddb0e: task_js_main at /mnt/d/project/robot/beshell/build/../main/task_js.c:220
0x4009337e: vPortTaskWrapper at /mnt/d/lib/esp-idf/components/freertos/port/xtensa/port.c:168
```