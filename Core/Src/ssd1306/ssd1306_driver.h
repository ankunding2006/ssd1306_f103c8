#ifndef SSD1306_DRIVER_H
#define SSD1306_DRIVER_H

#include "stm32f1xx_hal.h"
#include "ssd1306.h"

// 初始化SSD1306 OLED使用标准引脚配置 (PB6/PB7用于I2C1)
HAL_StatusTypeDef SSD1306_Driver_Init(void);

// 使用自定义引脚配置初始化SSD1306 OLED
HAL_StatusTypeDef SSD1306_Driver_Init_Custom(
    GPIO_TypeDef* scl_port, uint16_t scl_pin,
    GPIO_TypeDef* sda_port, uint16_t sda_pin,
    I2C_TypeDef* i2c_instance,
    DMA_Channel_TypeDef* dma_channel,
    uint32_t dma_irqn
);

#endif /* SSD1306_DRIVER_H */