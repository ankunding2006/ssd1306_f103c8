#ifndef SSD1306_DEFINES_H_
#define SSD1306_DEFINES_H_

// 不再需要定义I2C端口宏，直接在ssd1306.h中使用外部变量
// #define SSD1306_I2C_PORT  SSD1306_I2C_PORT   // 这行应该删除

#define SSD1306_ADDRESS    0x3C   // I2C address display
#define SSD1306_128X64            // SSD1306_128X32 or SSD1306_128X64
#define SSD1306_USE_DMA       1   // 0: not used I2C DMA mode, 1: used I2C DMA mode
#define SSD1306_CONTUPDATE    1   // 0: continue update mode disable, 1: continue update mode enable (only DMA MODE)

#endif /* SSD1306_DEFINES_H_ */