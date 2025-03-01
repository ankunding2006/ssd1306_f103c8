# SSD1306 OLED Driver Library

A standalone SSD1306 OLED display driver library for STM32F1 series microcontrollers supporting I2C interface and DMA transfers. This library works independently from STM32CubeMX generated code and can be integrated into any STM32F1 project.

## Features

- **Standalone operation**: No dependency on STM32CubeMX generated code
- **Flexible configuration**: Support for custom I2C interfaces, pins and DMA channels
- **DMA support**: Efficient display updates using DMA transfers
- **Interrupt-friendly**: Uses weak symbol definitions to avoid conflicts with user code
- **Error handling**: Includes I2C bus recovery mechanisms
- **Rich graphics API**: Functions for drawing points, lines, rectangles, circles and more
- **Font support**: Multiple built-in fonts for text display

## File Structure

```
/Drivers
    /SSD1306
        - ssd1306.c/h          # Core display functionality
        - ssd1306_defines.h     # Display-related definitions
        - ssd1306_hw.c/h        # Hardware abstraction layer
        - ssd1306_driver.c/h    # Driver initialization interface
        - ssd1306_it.c          # Interrupt handlers
        - fonts.c/h             # Font definitions
```

## Installation

1. Copy the `/Drivers/SSD1306` folder to your project
2. Add all `.c` files to your build system
3. Add `/Drivers/SSD1306` to your include paths

## API Reference

### Initialization Functions

```c
// Initialize SSD1306 with default configuration (I2C1, PB6/PB7, DMA1_Channel6)
HAL_StatusTypeDef SSD1306_Driver_Init(void);

// Initialize SSD1306 with custom configuration
HAL_StatusTypeDef SSD1306_Driver_Init_Custom(
    GPIO_TypeDef* scl_port, uint16_t scl_pin,
    GPIO_TypeDef* sda_port, uint16_t sda_pin,
    I2C_TypeDef* i2c_instance,
    DMA_Channel_TypeDef* dma_channel,
    uint32_t dma_irqn
);

// Release hardware resources
HAL_StatusTypeDef SSD1306_HW_DeInit(void);

// I2C bus recovery (handles bus hang-up conditions)
HAL_StatusTypeDef SSD1306_HW_BusRecovery(SSD1306_HW_Config_t* config);
```

### Display Control Functions

```c
// Get screen dimensions
uint16_t ssd1306_GetWidth(void);
uint16_t ssd1306_GetHeight(void);

// Set/get current color
void ssd1306_SetColor(SSD1306_COLOR color);
SSD1306_COLOR ssd1306_GetColor(void);

// Clear screen
void ssd1306_Clear(void);

// Set cursor position
void ssd1306_SetCursor(uint8_t x, uint8_t y);

// Update screen (send buffer contents to OLED)
void ssd1306_UpdateScreen(void);

// Check if update is complete
uint8_t ssd1306_UpdateScreenCompleted(void);
```

### Drawing Functions

```c
// Draw pixel
void ssd1306_DrawPixel(uint8_t x, uint8_t y);

// Draw lines
void ssd1306_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
void ssd1306_DrawHorizontalLine(int16_t x, int16_t y, int16_t length);
void ssd1306_DrawVerticalLine(int16_t x, int16_t y, int16_t length);

// Draw rectangles
void ssd1306_DrawRect(int16_t x, int16_t y, int16_t width, int16_t height);
void ssd1306_FillRect(int16_t x, int16_t y, int16_t width, int16_t height);

// Draw circles
void ssd1306_DrawCircle(int16_t x0, int16_t y0, int16_t radius);
void ssd1306_FillCircle(int16_t x0, int16_t y0, int16_t radius);

// Draw triangles
void ssd1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
void ssd1306_FillTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);

// Draw arcs
void ssd1306_DrawArc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep);

// Draw bitmaps
void ssd1306_DrawBitmap(uint8_t x, uint8_t y, uint8_t *bitmap, uint8_t w, uint8_t h);
```

### Text Functions

```c
// Display character
char ssd1306_WriteChar(char ch, FontDef Font);

// Display string
char ssd1306_WriteString(char* str, FontDef Font);
```

## Usage Examples

### Basic Example

Here's a complete example showing how to initialize the SSD1306 and display text and graphics:

```c
#include "stm32f1xx_hal.h"
#include "ssd1306_driver.h"
#include "fonts.h"

// System Clock Configuration
static void SystemClock_Config(void);

int main(void)
{
    // Initialize HAL library
    HAL_Init();
    
    // Configure system clock
    SystemClock_Config();
    
    // Initialize OLED display (with default config: I2C1, PB6/PB7, DMA1_Channel6)
    if (SSD1306_Driver_Init() != HAL_OK) {
        Error_Handler();
    }
    
    // Clear screen
    ssd1306_Clear();
    
    // Set color to white
    ssd1306_SetColor(White);
    
    // Display text
    ssd1306_SetCursor(10, 10);
    ssd1306_WriteString("SSD1306 OLED", Font_7x10);
    
    ssd1306_SetCursor(10, 30);
    ssd1306_WriteString("Hello World!", Font_7x10);
    
    // Draw rectangle border
    ssd1306_DrawRect(5, 5, 118, 40);
    
    // Draw circle
    ssd1306_DrawCircle(100, 50, 10);
    
    // Update screen
    ssd1306_UpdateScreen();
    
    // Main loop
    while (1)
    {
        HAL_Delay(100);
    }
}

// System Clock Configuration for 72MHz
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Configure HSE oscillator and PLL
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

    // Configure system clock
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
    // Error handling code
    __disable_irq();
    while (1)
    {
    }
}
```

### Custom Configuration Example

If you need to use different I2C interface or pins:

```c
// Use I2C2, pins PB10(SCL) and PB11(SDA), DMA1_Channel4
SSD1306_Driver_Init_Custom(
    GPIOB, GPIO_PIN_10,    // SCL port and pin
    GPIOB, GPIO_PIN_11,    // SDA port and pin
    I2C2,                  // I2C interface
    DMA1_Channel4,         // DMA channel
    DMA1_Channel4_IRQn     // DMA interrupt number
);
```

### Graphics Example

Display various graphical patterns:

```c
// Clear screen
ssd1306_Clear();

// Draw crossing lines
ssd1306_DrawLine(0, 0, 127, 63);
ssd1306_DrawLine(127, 0, 0, 63);

// Draw rectangle outline
ssd1306_DrawRect(20, 20, 40, 30);

// Draw filled rectangle
ssd1306_FillRect(70, 20, 40, 30);

// Draw circle
ssd1306_DrawCircle(40, 40, 15);

// Draw filled circle
ssd1306_FillCircle(90, 40, 15);

// Update display
ssd1306_UpdateScreen();
```

### Animation Example

Create a simple animation effect:

```c
int x = 0;
uint8_t direction = 1;

while (1)
{
    ssd1306_Clear();
    
    // Draw a moving circle
    ssd1306_FillCircle(x, 32, 10);
    
    // Update screen
    ssd1306_UpdateScreen();
    
    // Wait for update to complete
    while (!ssd1306_UpdateScreenCompleted())
    {
        // Can perform other tasks here
    }
    
    // Update circle position
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

### Bitmap Example

Display a custom bitmap:

```c
// Define an 8x8 bitmap (heart pattern)
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

// Display bitmap on screen
ssd1306_Clear();
ssd1306_DrawBitmap(60, 20, heart_bitmap, 8, 8);
ssd1306_UpdateScreen();
```

### Bus Recovery Example

If the I2C bus gets stuck, you can use the bus recovery function:

```c
// Prepare configuration structure
SSD1306_HW_Config_t config = {
    .SCL_Port = GPIOB,
    .SCL_Pin = GPIO_PIN_6,
    .SDA_Port = GPIOB,
    .SDA_Pin = GPIO_PIN_7,
    .I2C = I2C1,
    .DMA_Channel = DMA1_Channel6,
    .DMA_IRQn = DMA1_Channel6_IRQn
};

// Try to recover the bus
if (SSD1306_HW_BusRecovery(&config) != HAL_OK) {
    // Handle recovery failure
    Error_Handler();
}
```

## Interrupt Handling

The library uses weak symbol definitions for all necessary interrupt handlers, making it compatible with existing code. If your project already defines related interrupt handlers, you need to add SSD1306 interrupt handling to your functions.

For example:

```c
// If DMA interrupt handler already exists
void DMA1_Channel6_IRQHandler(void)
{
    // Your other DMA handling code
    
    // SSD1306's DMA handling
    if (SSD1306_DMA_CHANNEL == DMA1_Channel6) {
        HAL_DMA_IRQHandler(&hdma_i2c_tx);
    }
}

// If I2C event interrupt handler already exists
void I2C1_EV_IRQHandler(void)
{
    // Your other I2C handling code
    
    // SSD1306's I2C event handling
    if (SSD1306_I2C_INSTANCE == I2C1) {
        HAL_I2C_EV_IRQHandler(&SSD1306_I2C_PORT);
    }
}

void I2C1_ER_IRQHandler(void)
{
    // Your other I2C error handling code
    
    // SSD1306's I2C error handling
    if (SSD1306_I2C_INSTANCE == I2C1) {
        HAL_I2C_ER_IRQHandler(&SSD1306_I2C_PORT);
    }
}
```

## Advanced Configuration

### Changing Display Resolution

The default configuration supports 128x64 pixel displays. If using a 128x32 pixel display, modify in `ssd1306_defines.h`:

```c
// Use 128x32 display
// #define SSD1306_128X64
#define SSD1306_128X32
```

### Disabling DMA Mode

If you don't need DMA, you can disable it in `ssd1306_defines.h`:

```c
// Disable DMA mode
#define SSD1306_USE_DMA       0
```

### Changing I2C Address

If your OLED display uses a different I2C address (some modules use 0x3D), you can modify:

```c
// Change I2C address (if using 0x3D)
#define SSD1306_ADDRESS    0x3D
```

## Troubleshooting

### Display Not Lighting Up or Showing Abnormal Pattern

1. Check power connection and voltage (typically needs 3.3V)
2. Confirm the I2C address is correct (0x3C or 0x3D)
3. Verify SCL and SDA pins are connected properly
4. Try lowering I2C clock speed (modify `ClockSpeed` value in the `SSD1306_HW_Init` function, e.g., from 400000 to 100000)
5. Use a logic analyzer or oscilloscope to check I2C signals
6. Call `SSD1306_HW_BusRecovery` function to attempt bus recovery

### Display Content Not Updating

1. Make sure `ssd1306_UpdateScreen()` is called after drawing operations
2. Check if interrupts are correctly configured (DMA mode requires interrupt support)
3. If using DMA mode, check the return value of `ssd1306_UpdateScreenCompleted()` to confirm update completion

### Compilation Errors

1. Make sure all necessary files are included
2. Check if HAL library is configured correctly
3. Ensure there are no duplicate function or variable definitions (especially for interrupt handlers)

## Performance Optimization

1. Use DMA screen updates to reduce CPU usage
2. Update only regions that need to change, not the entire screen
3. Use font rendering cache if displaying the same text frequently
4. For static content, pre-generate bitmaps rather than drawing in real-time

## License

This project is released under the MIT License.

## Acknowledgements

- Original SSD1306 driver based on work by Andriy Honcharenko and Roberto Benjami
- Hardware abstraction layer and standalone initialization features added by ankunding from Chian University of Mining and Technology