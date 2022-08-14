### 设备驱动 (driver)

设备驱动类(JS Class) 需要实现4个函数

* setup(opts)
* begin(opts)
* end()
* unsetup()

以 SD 为例：

* setup() 初始化 spi设备(不负责初始化spi总线)
* begin() 挂载 SD 上的文件系统
* end() 取消挂载文件系统
* unsetup() 注销 mcu 上的 spi 外设

### 零件 （part）

  
* 一个零件可以包含1个或多个驱动

* 零件分为 `主机` 和 `外设`

#### 命名方式

* 芯片的驱动名称全部大写，例如： ST7789, ES8156 

* 通用驱动名称全部小写，可以用中线`-` 分隔单词，例如： rgb-led, sdspi 

* 零件名称采用驼峰，BECOME 第一方零件产品以 `Be` 开头，例如： BeScreen096、 BeSDSlot 