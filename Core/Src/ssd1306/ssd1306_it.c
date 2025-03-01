#include "stm32f1xx_hal.h"
#include "ssd1306_hw.h"

// 这些声明需要与ssd1306.c中相匹配
extern volatile uint8_t ssd1306_updatestatus;
extern volatile uint8_t ssd1306_updateend;

/* 通用中断处理函数 - 弱符号实现 */
// DMA中断处理函数 - 用户可以在自己的代码中覆盖这些函数
__weak void DMA1_Channel1_IRQHandler(void) { 
    if(SSD1306_DMA_CHANNEL == DMA1_Channel1)
        HAL_DMA_IRQHandler(&hdma_i2c_tx); 
}

__weak void DMA1_Channel2_IRQHandler(void) { 
    if(SSD1306_DMA_CHANNEL == DMA1_Channel2) 
        HAL_DMA_IRQHandler(&hdma_i2c_tx); 
}

__weak void DMA1_Channel3_IRQHandler(void) { 
    if(SSD1306_DMA_CHANNEL == DMA1_Channel3) 
        HAL_DMA_IRQHandler(&hdma_i2c_tx); 
}

__weak void DMA1_Channel4_IRQHandler(void) { 
    if(SSD1306_DMA_CHANNEL == DMA1_Channel4) 
        HAL_DMA_IRQHandler(&hdma_i2c_tx); 
}

__weak void DMA1_Channel5_IRQHandler(void) { 
    if(SSD1306_DMA_CHANNEL == DMA1_Channel5) 
        HAL_DMA_IRQHandler(&hdma_i2c_tx); 
}

__weak void DMA1_Channel6_IRQHandler(void) { 
    if(SSD1306_DMA_CHANNEL == DMA1_Channel6) 
        HAL_DMA_IRQHandler(&hdma_i2c_tx); 
}

__weak void DMA1_Channel7_IRQHandler(void) { 
    if(SSD1306_DMA_CHANNEL == DMA1_Channel7) 
        HAL_DMA_IRQHandler(&hdma_i2c_tx); 
}

// I2C事件中断处理
__weak void I2C1_EV_IRQHandler(void) {
    if(SSD1306_I2C_INSTANCE == I2C1)
        HAL_I2C_EV_IRQHandler(&SSD1306_I2C_PORT);
}

__weak void I2C1_ER_IRQHandler(void) {
    if(SSD1306_I2C_INSTANCE == I2C1)
        HAL_I2C_ER_IRQHandler(&SSD1306_I2C_PORT);
}

__weak void I2C2_EV_IRQHandler(void) {
    if(SSD1306_I2C_INSTANCE == I2C2)
        HAL_I2C_EV_IRQHandler(&SSD1306_I2C_PORT);
}

__weak void I2C2_ER_IRQHandler(void) {
    if(SSD1306_I2C_INSTANCE == I2C2)
        HAL_I2C_ER_IRQHandler(&SSD1306_I2C_PORT);
}