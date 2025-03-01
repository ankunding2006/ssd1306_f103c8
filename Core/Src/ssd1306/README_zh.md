# SSD1306 OLED 驱动库

这是一个为STM32F1系列微控制器设计的SSD1306 OLED驱动库，支持I2C接口和DMA传输。该库的特点是不依赖于STM32CubeMX生成的代码，可以作为独立模块集成到任何STM32F1项目中。

## 特点

- **独立运行**：不依赖STM32CubeMX生成的代码
- **灵活配置**：支持自定义I2C接口、引脚和DMA通道
- **DMA支持**：使用DMA提高显示更新效率
- **中断友好**：使用弱符号定义，避免与用户代码冲突
- **错误处理**：包含I2C总线恢复功能
- **丰富图形API**：提供点、线、矩形、圆等基本图形绘制功能
- **字体支持**：内置多种字体，支持文本显示

## 文件结构

```
/Drivers
    /SSD1306
        - ssd1306.c/h          # 核心显示功能实现
        - ssd1306_defines.h     # 显示相关定义
        - ssd1306_hw.c/h        # 硬件抽象层
        - ssd1306_driver.c/h    # 驱动初始化接口
        - ssd1306_it.c          # 中断处理
        - fonts.c/h             # 字体定义
```

## 库安装

1. 将`/Drivers/SSD1306`文件夹复制到您的项目中
2. 将所有`.c`文件添加到您的构建系统
3. 将`/Drivers/SSD1306`添加到包含路径

## API参考

### 初始化函数

```c
// 使用默认配置初始化SSD1306 (I2C1, PB6/PB7, DMA1_Channel6)
HAL_StatusTypeDef SSD1306_Driver_Init(void);

// 使用自定义配置初始化SSD1306
HAL_StatusTypeDef SSD1306_Driver_Init_Custom(
    GPIO_TypeDef* scl_port, uint16_t scl_pin,
    GPIO_TypeDef* sda_port, uint16_t sda_pin,
    I2C_TypeDef* i2c_instance,
    DMA_Channel_TypeDef* dma_channel,
    uint32_t dma_irqn
);

// 释放硬件资源
HAL_StatusTypeDef SSD1306_HW_DeInit(void);

// I2C总线恢复（处理总线卡死情况）
HAL_StatusTypeDef SSD1306_HW_BusRecovery(SSD1306_HW_Config_t* config);
```

### 显示控制函数

```c
// 获取屏幕尺寸
uint16_t ssd1306_GetWidth(void);
uint16_t ssd1306_GetHeight(void);

// 设置/获取当前颜色
void ssd1306_SetColor(SSD1306_COLOR color);
SSD1306_COLOR ssd1306_GetColor(void);

// 清空屏幕
void ssd1306_Clear(void);

// 设置光标位置
void ssd1306_SetCursor(uint8_t x, uint8_t y);

// 更新屏幕显示（将缓冲区内容发送到OLED）
void ssd1306_UpdateScreen(void);

// 检查更新是否完成
uint8_t ssd1306_UpdateScreenCompleted(void);
```

### 绘图函数

```c
// 绘制像素
void ssd1306_DrawPixel(uint8_t x, uint8_t y);

// 绘制线段
void ssd1306_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
void ssd1306_DrawHorizontalLine(int16_t x, int16_t y, int16_t length);
void ssd1306_DrawVerticalLine(int16_t x, int16_t y, int16_t length);

// 绘制矩形
void ssd1306_DrawRect(int16_t x, int16_t y, int16_t width, int16_t height);
void ssd1306_FillRect(int16_t x, int16_t y, int16_t width, int16_t height);

// 绘制圆形
void ssd1306_DrawCircle(int16_t x0, int16_t y0, int16_t radius);
void ssd1306_FillCircle(int16_t x0, int16_t y0, int16_t radius);

// 绘制三角形
void ssd1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
void ssd1306_FillTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);

// 绘制圆弧
void ssd1306_DrawArc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep);

// 绘制位图
void ssd1306_DrawBitmap(uint8_t x, uint8_t y, uint8_t *bitmap, uint8_t w, uint8_t h);
```

### 文本函数

```c
// 显示字符
char ssd1306_WriteChar(char ch, FontDef Font);

// 显示字符串
char ssd1306_WriteString(char* str, FontDef Font);
```

## 使用示例

### 基本示例

下面是一个完整的示例，展示如何初始化SSD1306并显示文本和图形：

```c
#include "stm32f1xx_hal.h"
#include "ssd1306_driver.h"
#include "fonts.h"

// 系统时钟配置
static void SystemClock_Config(void);

int main(void)
{
    // 初始化HAL库
    HAL_Init();
    
    // 配置系统时钟
    SystemClock_Config();
    
    // 初始化OLED显示器 (使用默认配置: I2C1, PB6/PB7, DMA1_Channel6)
    if (SSD1306_Driver_Init() != HAL_OK) {
        Error_Handler();
    }
    
    // 清屏
    ssd1306_Clear();
    
    // 设置颜色为白色
    ssd1306_SetColor(White);
    
    // 显示文本
    ssd1306_SetCursor(10, 10);
    ssd1306_WriteString("SSD1306 OLED", Font_7x10);
    
    ssd1306_SetCursor(10, 30);
    ssd1306_WriteString("Hello World!", Font_7x10);
    
    // 绘制矩形边框
    ssd1306_DrawRect(5, 5, 118, 40);
    
    // 绘制圆形
    ssd1306_DrawCircle(100, 50, 10);
    
    // 更新屏幕
    ssd1306_UpdateScreen();
    
    // 主循环
    while (1)
    {
        HAL_Delay(100);
    }
}

// 系统时钟配置为72MHz
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // 配置HSE振荡器和PLL
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    // 配置系统时钟
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

void Error_Handler(void)
{
    // 错误处理代码
    __disable_irq();
    while (1)
    {
    }
}
```

### 自定义配置示例

如果需要使用不同的I2C接口或引脚：

```c
// 使用I2C2，引脚PB10(SCL)和PB11(SDA)，DMA1_Channel4
SSD1306_Driver_Init_Custom(
    GPIOB, GPIO_PIN_10,    // SCL引脚
    GPIOB, GPIO_PIN_11,    // SDA引脚
    I2C2,                  // I2C接口
    DMA1_Channel4,         // DMA通道
    DMA1_Channel4_IRQn     // DMA中断号
);
```

### 图形示例

显示各种图形图案：

```c
// 清屏
ssd1306_Clear();

// 绘制交叉线
ssd1306_DrawLine(0, 0, 127, 63);
ssd1306_DrawLine(127, 0, 0, 63);

// 绘制矩形框
ssd1306_DrawRect(20, 20, 40, 30);

// 绘制填充矩形
ssd1306_FillRect(70, 20, 40, 30);

// 绘制圆
ssd1306_DrawCircle(40, 40, 15);

// 绘制填充圆
ssd1306_FillCircle(90, 40, 15);

// 更新显示
ssd1306_UpdateScreen();
```

### 动画示例

创建简单的动画效果：

```c
int x = 0;
uint8_t direction = 1;

while (1)
{
    ssd1306_Clear();
    
    // 绘制一个移动的圆
    ssd1306_FillCircle(x, 32, 10);
    
    // 更新屏幕
    ssd1306_UpdateScreen();
    
    // 等待更新完成
    while (!ssd1306_UpdateScreenCompleted())
    {
        // 可以执行其他任务
    }
    
    // 移动圆的位置
    if (direction)
    {
        x++;
        if (x >= 118) direction = 0;
    }
    else
    {
        x--;
        if (x <= 10) direction = 1;
    }
    
    HAL_Delay(10);
}
```

### 显示位图示例

显示自定义位图：

```c
// 定义一个8x8的位图（心形图案）
const uint8_t heart_bitmap[] = {
    0b00000000,
    0b01100110,
    0b11111111,
    0b11111111,
    0b01111110,
    0b00111100,
    0b00011000,
    0b00000000
};

// 在屏幕上显示位图
ssd1306_Clear();
ssd1306_DrawBitmap(60, 20, heart_bitmap, 8, 8);
ssd1306_UpdateScreen();
```

### 总线恢复示例

如果I2C总线卡死，可以使用总线恢复功能：

```c
// 准备配置结构体
SSD1306_HW_Config_t config = {
    .SCL_Port = GPIOB,
    .SCL_Pin = GPIO_PIN_6,
    .SDA_Port = GPIOB,
    .SDA_Pin = GPIO_PIN_7,
    .I2C = I2C1,
    .DMA_Channel = DMA1_Channel6,
    .DMA_IRQn = DMA1_Channel6_IRQn
};

// 尝试恢复总线
if (SSD1306_HW_BusRecovery(&config) != HAL_OK) {
    // 处理恢复失败的情况
    Error_Handler();
}
```

## 中断处理

库使用弱符号定义了所有必要的中断处理函数，使其能与现有代码兼容。如果你的项目中已经定义了相关中断处理函数，需要在你的函数中添加对SSD1306中断的处理。

例如：

```c
// 如果已存在DMA中断处理函数
void DMA1_Channel6_IRQHandler(void)
{
    // 你的其他DMA处理代码
    
    // SSD1306的DMA处理
    if (SSD1306_DMA_CHANNEL == DMA1_Channel6) {
        HAL_DMA_IRQHandler(&hdma_i2c_tx);
    }
}

// 如果已存在I2C中断处理函数
void I2C1_EV_IRQHandler(void)
{
    // 你的其他I2C处理代码
    
    // SSD1306的I2C事件处理
    if (SSD1306_I2C_INSTANCE == I2C1) {
        HAL_I2C_EV_IRQHandler(&SSD1306_I2C_PORT);
    }
}

void I2C1_ER_IRQHandler(void)
{
    // 你的其他I2C错误处理代码
    
    // SSD1306的I2C错误处理
    if (SSD1306_I2C_INSTANCE == I2C1) {
        HAL_I2C_ER_IRQHandler(&SSD1306_I2C_PORT);
    }
}
```

## 高级配置

### 修改显示器分辨率

默认配置支持128x64像素显示器。如果使用128x32像素显示器，可以在`ssd1306_defines.h`中修改：

```c
// 使用128x32显示器
// #define SSD1306_128X64
#define SSD1306_128X32
```

### 禁用DMA模式

如果不需要使用DMA，可以在`ssd1306_defines.h`中禁用：

```c
// 禁用DMA模式
#define SSD1306_USE_DMA       0
```

### 修改I2C地址

如果OLED显示器使用不同的I2C地址（某些模块使用0x3D），可以修改：

```c
// 修改I2C地址（如果是0x3D）
#define SSD1306_ADDRESS    0x3D
```

## 疑难解答

### 显示屏不亮或显示异常

1. 检查电源连接和电压（通常需要3.3V）
2. 确认I2C地址是否正确（0x3C或0x3D）
3. 确认SCL和SDA引脚连接正确
4. 尝试降低I2C时钟速度（修改`SSD1306_HW_Init`函数中的`ClockSpeed`值，如：从400000降至100000）
5. 使用逻辑分析仪或示波器检查I2C信号
6. 调用`SSD1306_HW_BusRecovery`函数尝试恢复总线

### 显示内容不更新

1. 确保每次绘图操作后调用了`ssd1306_UpdateScreen()`
2. 检查中断是否正确配置（DMA模式需要中断支持）
3. 如果使用DMA模式，检查`ssd1306_UpdateScreenCompleted()`的返回值，确认更新已完成

### 编译错误

1. 确保包含了所有必要的文件
2. 检查HAL库配置是否正确
3. 确保没有重复定义的函数或变量（特别是中断处理函数）

## 性能优化

1. 使用DMA更新屏幕以减少CPU占用
2. 只更新需要更改的区域，而不是整个屏幕
3. 使用字体渲染缓存（如果要频繁显示相同文字）
4. 对于静态内容，预先生成位图而不是实时绘制

## 许可

本项目基于MIT许可证发布。

## 致谢

- 原始SSD1306驱动基于Andriy Honcharenko和Roberto Benjami的工作
- 硬件抽象层和独立初始化功能由 来自中国矿业大学的  ankunding 添加
