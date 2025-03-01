#ifndef SSD1306_HW_H
#define SSD1306_HW_H

#include "stm32f1xx_hal.h"

// 硬件配置结构体
typedef struct {
    GPIO_TypeDef* SCL_Port;
    uint16_t SCL_Pin;
    GPIO_TypeDef* SDA_Port;
    uint16_t SDA_Pin;
    I2C_TypeDef* I2C;
    DMA_Channel_TypeDef* DMA_Channel;
    uint32_t DMA_IRQn;
} SSD1306_HW_Config_t;

// 导出I2C句柄供SSD1306驱动使用
extern I2C_HandleTypeDef SSD1306_I2C_PORT;
extern DMA_HandleTypeDef hdma_i2c_tx;

// 硬件初始化函数
HAL_StatusTypeDef SSD1306_HW_Init(SSD1306_HW_Config_t* config);

// 总线恢复函数
HAL_StatusTypeDef SSD1306_HW_BusRecovery(SSD1306_HW_Config_t* config);

// 资源释放函数
HAL_StatusTypeDef SSD1306_HW_DeInit(void);

// 中断处理相关宏定义
// 这些宏会在编译时根据实际配置生成正确的中断处理函数名称
#define DMA1_CH1_HANDLER DMA1_Channel1_IRQHandler
#define DMA1_CH2_HANDLER DMA1_Channel2_IRQHandler
#define DMA1_CH3_HANDLER DMA1_Channel3_IRQHandler
#define DMA1_CH4_HANDLER DMA1_Channel4_IRQHandler
#define DMA1_CH5_HANDLER DMA1_Channel5_IRQHandler
#define DMA1_CH6_HANDLER DMA1_Channel6_IRQHandler
#define DMA1_CH7_HANDLER DMA1_Channel7_IRQHandler

#define I2C1_EV_HANDLER I2C1_EV_IRQHandler
#define I2C1_ER_HANDLER I2C1_ER_IRQHandler
#define I2C2_EV_HANDLER I2C2_EV_IRQHandler
#define I2C2_ER_HANDLER I2C2_ER_IRQHandler

// 全局变量记录当前使用的通道
extern DMA_Channel_TypeDef* SSD1306_DMA_CHANNEL;
extern I2C_TypeDef* SSD1306_I2C_INSTANCE;

#endif /* SSD1306_HW_H */