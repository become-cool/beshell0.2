beapi.i2c.setup(0,0,2)
beapi.i2c.setup(0,0,2)


const MSMode_MasterSelOn = 0				//产品主从模式选择:默认选择0为SlaveMode,打开为1选择MasterMode
const Ratio = 256				//实际Ratio=MCLK/LRCK比率,需要和实际时钟比例匹配
const Format = NORMAL_I2S		//数据格式选择,需要和实际时序匹配
const Format_Len = Format_Len16	//数据长度选择,需要和实际时序匹配
const SCLK_DIV = 4				//SCLK分频选择:(选择范围1~128),SCLK=MCLK/SCLK_DIV
const SCLK_INV = 0				//默认对齐方式为下降沿,1为上升沿对齐,需要和实际时序匹配
const MCLK_SOURCE = MCLK_PIN		//是否硬件没接MCLK需要用SCLK当作MCLK
const EQ7bandOn = 0				//开启七段EQ下配置1，三段或者不使用配置0
const VDDA_VOLTAGE = VDDA_3V3		//模拟电压选择为3V3还是1V8,需要和实际硬件匹配
const DAC_Volume = 191				//DAC数字增益:(选择范围0~255),191:0DB,±0.5dB/Step
const DACHPModeOn = 0				//输出负载开启HP驱动:默认选择关闭0,打开为1

beapi.i2c.send(0, 0x8, [0x02,(MCLK_SOURCE<<7) + (SCLK_INV<<4) +  (EQ7bandOn<<3) + 0x04 + MSMode_MasterSelOn])
