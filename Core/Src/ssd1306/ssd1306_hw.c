#include "ssd1306_hw.h"

// 创建I2C句柄
I2C_HandleTypeDef SSD1306_I2C_PORT;

// DMA句柄
DMA_HandleTypeDef hdma_i2c_tx;

// 记录当前使用的通道和实例
DMA_Channel_TypeDef* SSD1306_DMA_CHANNEL = NULL;
I2C_TypeDef* SSD1306_I2C_INSTANCE = NULL;

HAL_StatusTypeDef SSD1306_HW_Init(SSD1306_HW_Config_t* config)
{
    HAL_StatusTypeDef status;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 保存当前配置以供中断处理使用
    SSD1306_DMA_CHANNEL = config->DMA_Channel;
    SSD1306_I2C_INSTANCE = config->I2C;
    
    // 1. 使能外设时钟
    // 使能GPIO时钟
    if(config->SCL_Port == GPIOA)
        __HAL_RCC_GPIOA_CLK_ENABLE();
    else if(config->SCL_Port == GPIOB)
        __HAL_RCC_GPIOB_CLK_ENABLE();
    else if(config->SCL_Port == GPIOC)
        __HAL_RCC_GPIOC_CLK_ENABLE();
    else if(config->SCL_Port == GPIOD)
        __HAL_RCC_GPIOD_CLK_ENABLE();
    
    // 如果SDA不在同一个端口，则再次启用时钟
    if(config->SDA_Port != config->SCL_Port) {
        if(config->SDA_Port == GPIOA)
            __HAL_RCC_GPIOA_CLK_ENABLE();
        else if(config->SDA_Port == GPIOB)
            __HAL_RCC_GPIOB_CLK_ENABLE();
        else if(config->SDA_Port == GPIOC)
            __HAL_RCC_GPIOC_CLK_ENABLE();
        else if(config->SDA_Port == GPIOD)
            __HAL_RCC_GPIOD_CLK_ENABLE();
    }
    
    // 使能I2C时钟
    if(config->I2C == I2C1)
        __HAL_RCC_I2C1_CLK_ENABLE();
    else if(config->I2C == I2C2)
        __HAL_RCC_I2C2_CLK_ENABLE();
    
    // 使能DMA时钟
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    // 使能AFIO时钟(用于引脚重映射)
    __HAL_RCC_AFIO_CLK_ENABLE();
    
    // 2. 配置GPIO引脚
    GPIO_InitStruct.Pin = config->SCL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(config->SCL_Port, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = config->SDA_Pin;
    HAL_GPIO_Init(config->SDA_Port, &GPIO_InitStruct);
    
    // 3. 初始化DMA
    hdma_i2c_tx.Instance = config->DMA_Channel;
    hdma_i2c_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_i2c_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c_tx.Init.Mode = DMA_NORMAL;
    hdma_i2c_tx.Init.Priority = DMA_PRIORITY_LOW;
    
    if (HAL_DMA_Init(&hdma_i2c_tx) != HAL_OK)
    {
      return HAL_ERROR;
    }
    
    // 4. 配置DMA中断
    HAL_NVIC_SetPriority(config->DMA_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(config->DMA_IRQn);
    
    // 5. 配置I2C
    SSD1306_I2C_PORT.Instance = config->I2C;
    SSD1306_I2C_PORT.Init.ClockSpeed = 400000;  // 400 kHz
    SSD1306_I2C_PORT.Init.DutyCycle = I2C_DUTYCYCLE_2;
    SSD1306_I2C_PORT.Init.OwnAddress1 = 0;
    SSD1306_I2C_PORT.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    SSD1306_I2C_PORT.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    SSD1306_I2C_PORT.Init.OwnAddress2 = 0;
    SSD1306_I2C_PORT.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    SSD1306_I2C_PORT.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    
    status = HAL_I2C_Init(&SSD1306_I2C_PORT);
    if (status != HAL_OK)
        return status;
    
    // 6. 链接DMA与I2C
    __HAL_LINKDMA(&SSD1306_I2C_PORT, hdmatx, hdma_i2c_tx);
    
    // 7. 配置I2C中断
    if(config->I2C == I2C1) {
        HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
        HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
    } 
    else if(config->I2C == I2C2) {
        HAL_NVIC_SetPriority(I2C2_EV_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
        HAL_NVIC_SetPriority(I2C2_ER_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
    }
    
    // 8. 验证I2C总线是否正常
    // 添加可选的总线恢复逻辑
    // 如果总线卡住，可以尝试手动切换SCL几次来释放总线
    
    return HAL_OK;
}

// 添加总线恢复函数 - 如果I2C总线卡住可以调用此函数
HAL_StatusTypeDef SSD1306_HW_BusRecovery(SSD1306_HW_Config_t* config)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 暂时将I2C引脚配置为普通输出
    GPIO_InitStruct.Pin = config->SCL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(config->SCL_Port, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = config->SDA_Pin;
    HAL_GPIO_Init(config->SDA_Port, &GPIO_InitStruct);
    
    // 设置SDA为高电平
    HAL_GPIO_WritePin(config->SDA_Port, config->SDA_Pin, GPIO_PIN_SET);
    
    // 切换SCL 9次，使从设备释放SDA线
    for(int i = 0; i < 9; i++) {
        HAL_GPIO_WritePin(config->SCL_Port, config->SCL_Pin, GPIO_PIN_RESET);
        HAL_Delay(1);
        HAL_GPIO_WritePin(config->SCL_Port, config->SCL_Pin, GPIO_PIN_SET);
        HAL_Delay(1);
    }
    
    // 生成一个STOP条件
    HAL_GPIO_WritePin(config->SCL_Port, config->SCL_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(config->SDA_Port, config->SDA_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(config->SDA_Port, config->SDA_Pin, GPIO_PIN_SET);
    
    // 将引脚重新配置为I2C模式
    GPIO_InitStruct.Pin = config->SCL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    HAL_GPIO_Init(config->SCL_Port, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = config->SDA_Pin;
    HAL_GPIO_Init(config->SDA_Port, &GPIO_InitStruct);
    
    // 重新初始化I2C并返回状态
    HAL_StatusTypeDef status = HAL_I2C_Init(&SSD1306_I2C_PORT);
    if (status != HAL_OK) {
        return status;
    }
    
    // 尝试验证设备是否可用
    status = HAL_I2C_IsDeviceReady(&SSD1306_I2C_PORT, (0x3C << 1), 3, 100);
    return status;
}

// 添加DeInit函数以支持资源释放
HAL_StatusTypeDef SSD1306_HW_DeInit(void)
{
    // 禁用相关中断
    if(SSD1306_I2C_INSTANCE == I2C1) {
        HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
        HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
    } 
    else if(SSD1306_I2C_INSTANCE == I2C2) {
        HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);
        HAL_NVIC_DisableIRQ(I2C2_ER_IRQn);
    }
    
    // 查找对应的DMA中断并禁用
    // 注意：这里简化处理，实际项目中应根据具体DMA通道确定IRQn
        // 查找对应的DMA中断并禁用
    if(SSD1306_DMA_CHANNEL) {
        if(SSD1306_DMA_CHANNEL == DMA1_Channel1) {
            HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);
        } else if(SSD1306_DMA_CHANNEL == DMA1_Channel2) {
            HAL_NVIC_DisableIRQ(DMA1_Channel2_IRQn);
        } else if(SSD1306_DMA_CHANNEL == DMA1_Channel3) {
            HAL_NVIC_DisableIRQ(DMA1_Channel3_IRQn);
        } else if(SSD1306_DMA_CHANNEL == DMA1_Channel4) {
            HAL_NVIC_DisableIRQ(DMA1_Channel4_IRQn);
        } else if(SSD1306_DMA_CHANNEL == DMA1_Channel5) {
            HAL_NVIC_DisableIRQ(DMA1_Channel5_IRQn);
        } else if(SSD1306_DMA_CHANNEL == DMA1_Channel6) {
            HAL_NVIC_DisableIRQ(DMA1_Channel6_IRQn);
        } else if(SSD1306_DMA_CHANNEL == DMA1_Channel7) {
            HAL_NVIC_DisableIRQ(DMA1_Channel7_IRQn);
        }
    }
    // 反初始化I2C
    HAL_I2C_DeInit(&SSD1306_I2C_PORT);
    
    // 反初始化DMA
    HAL_DMA_DeInit(&hdma_i2c_tx);
    
    // 清除实例记录
    SSD1306_DMA_CHANNEL = NULL;
    SSD1306_I2C_INSTANCE = NULL;
    
    return HAL_OK;
}