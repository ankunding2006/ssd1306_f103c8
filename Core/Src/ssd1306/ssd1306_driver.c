#include "ssd1306_driver.h"
#include "ssd1306_hw.h"

HAL_StatusTypeDef SSD1306_Driver_Init(void)
{
    // 使用标准配置: PB6/PB7 作为I2C1
    SSD1306_HW_Config_t config = {
        .SCL_Port = GPIOB,
        .SCL_Pin = GPIO_PIN_6,
        .SDA_Port = GPIOB,
        .SDA_Pin = GPIO_PIN_7,
        .I2C = I2C1,
        .DMA_Channel = DMA1_Channel6,
        .DMA_IRQn = DMA1_Channel6_IRQn
    };
    
    HAL_StatusTypeDef status = SSD1306_HW_Init(&config);
    if (status != HAL_OK)
        return status;
    
    // 初始化OLED本身
    if (!ssd1306_Init())
        return HAL_ERROR;
    
    return HAL_OK;
}

HAL_StatusTypeDef SSD1306_Driver_Init_Custom(
    GPIO_TypeDef* scl_port, uint16_t scl_pin,
    GPIO_TypeDef* sda_port, uint16_t sda_pin,
    I2C_TypeDef* i2c_instance,
    DMA_Channel_TypeDef* dma_channel,
    uint32_t dma_irqn)
{
    // 验证参数
    if (scl_port == NULL || sda_port == NULL || i2c_instance == NULL || dma_channel == NULL) {
        return HAL_ERROR;
    }
    SSD1306_HW_Config_t config = {
        .SCL_Port = scl_port,
        .SCL_Pin = scl_pin,
        .SDA_Port = sda_port,
        .SDA_Pin = sda_pin,
        .I2C = i2c_instance,
        .DMA_Channel = dma_channel,
        .DMA_IRQn = dma_irqn
    };
    
    HAL_StatusTypeDef status = SSD1306_HW_Init(&config);
    if (status != HAL_OK)
        return status;
    
    // 初始化OLED本身
    if (!ssd1306_Init())
        return HAL_ERROR;
    
    return HAL_OK;
}