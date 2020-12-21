## 编译

```
source esp-idf/export.sh
./make
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
    python esp-idf/components/esptool_py/esptool/espefuse.py --port COM8 set_flash_voltage 3.3V
    ```

官方说明：
```
https://github.com/espressif/esp-idf/tree/master/examples/storage/sd_card#note-about-gpio12-esp32-only
```

efuses 是一次性编程，不能重复修改

## 无限重启

esp32 启动时电流较大，sgm2019 很容易支持不了，导致无限重启，可以在 3V3 和 GND 之间加一个大电容 (47uF)

```

```

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

## Esp32版本 无法连接 SD

执行 `E.connectSDCard()` 函数报错：

```
Uncaught Error: Unimplemented on Linux
```


解决方案详见:

https://github.com/espruino/Espruino/issues/1779



## eFuse

工具文档：

```
https://github.com/espressif/esptool/wiki/espefuse
```

可在出厂时将产品型号写入 efuse ，在 js 运行时读取

> esp_idf c++ 读取 efuse :
>
> API: https://github.com/espressif/esp-idf/tree/master/components/efuse
>
> 例子 : https://github.com/espressif/esp-idf/blob/master/examples/system/efuse