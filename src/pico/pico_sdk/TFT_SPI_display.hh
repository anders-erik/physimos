
#pragma once

#include "hardware/spi.h"

#define SPI_PORT spi0

// IO PINS          // Name printed next to physical display pins
#define PIN_MISO 16 // SDA
#define PIN_CS   17 // CD
#define PIN_SCK  18 // SCK
#define PIN_MOSI 19 //  - (PIN NOT PRESENT)
#define PIN_DC   21 // A0
#define PIN_RST  20 // RESET

// COMMANDS 
#define ST7735_SWRESET 0x01 // return internal registers to their default states
#define ST7735_SLPOUT  0x11 // turn off sleep mode, thus starting the display
#define ST7735_COLMOD  0x3A // set pixel bit depth/format
#define ST7735_MADCTL  0x36 // display control ?
#define ST7735_DISPON  0x29 // display on
#define ST7735_CASET   0x2A // Set column   [start, end]
#define ST7735_RASET   0x2B // Set row      [start, end]
#define ST7735_RAMWR   0x2C // RAM Write

// MISC
#define ST7735_RGB565               0x05 // 16-bit/pixel (RGB565) value
#define ST7735_DEFAULT_ORIENTATION  0x00 // default orientation

// DIMS
#define DISPLAY_WIDTH  128
#define DISPLAY_HEIGHT 160

static inline void cs_select()   { gpio_put(PIN_CS, 0); }
static inline void cs_deselect() { gpio_put(PIN_CS, 1); }

/** ST7735 specific commands I believe.. */
static void st7735_write_command(uint8_t cmd)
{
    gpio_put(PIN_DC, 0); // command
    cs_select();
    spi_write_blocking(SPI_PORT, &cmd, 1);
    cs_deselect();
}

/** Write pixel data */
static void write_data(const uint8_t *data, size_t len)
{
    gpio_put(PIN_DC, 1); // data
    cs_select();
    spi_write_blocking(SPI_PORT, data, len);
    cs_deselect();
}

static void write_data_byte(uint8_t b)
{
    write_data(&b, 1);
}

static void st7735_reset()
{
    gpio_put(PIN_RST, 1);
    sleep_ms(10);
    gpio_put(PIN_RST, 0);
    sleep_ms(10);
    gpio_put(PIN_RST, 1);
    sleep_ms(120);
}

static void st7735_init()
{
    st7735_reset();

    st7735_write_command(ST7735_SWRESET);
    sleep_ms(150);

    st7735_write_command(ST7735_SLPOUT);
    sleep_ms(200);

    st7735_write_command(ST7735_COLMOD);
    write_data_byte(ST7735_RGB565);
    sleep_ms(10);

    st7735_write_command(ST7735_MADCTL);
    write_data_byte(ST7735_DEFAULT_ORIENTATION);

    st7735_write_command(ST7735_DISPON);
    sleep_ms(100);
}

// Sets the active address window (inclusive coordinates) that RAMWR will write into.
static void set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint8_t col_data[4] = { (uint8_t)(x0 >> 8), (uint8_t)(x0 & 0xFF), (uint8_t)(x1 >> 8), (uint8_t)(x1 & 0xFF) };
    uint8_t row_data[4] = { (uint8_t)(y0 >> 8), (uint8_t)(y0 & 0xFF), (uint8_t)(y1 >> 8), (uint8_t)(y1 & 0xFF) };

    st7735_write_command(ST7735_CASET);
    write_data(col_data, sizeof(col_data));

    st7735_write_command(ST7735_RASET);
    write_data(row_data, sizeof(row_data));
}

// Sets the address window to a single pixel at (x, y) and writes one RGB565 color.
// static void st7735_set_pixel(uint16_t x, uint16_t y, uint16_t color565)
// {
//     uint8_t pixel_data[2] = { (uint8_t)(color565 >> 8), (uint8_t)(color565 & 0xFF) };

//     st7735_set_window(x, y, x, y);

//     st7735_write_command(ST7735_RAMWR);
//     st7735_write_data(pixel_data, sizeof(pixel_data));
// }

// Fills the whole screen with a single RGB565 color.
// Keeps CS asserted for the entire transfer instead of toggling it per pixel.
static void fill_screen(uint16_t color565)
{
    uint8_t row_buf[DISPLAY_WIDTH * 2];
    uint8_t hi = (uint8_t)(color565 >> 8);
    uint8_t lo = (uint8_t)(color565 & 0xFF);
    for (int i = 0; i < DISPLAY_WIDTH; i++)
    {
        row_buf[i * 2]     = hi;
        row_buf[i * 2 + 1] = lo;
    }

    set_window(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);
    st7735_write_command(ST7735_RAMWR);

    gpio_put(PIN_DC, 1); // data
    cs_select();
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        spi_write_blocking(SPI_PORT, row_buf, sizeof(row_buf));
    }
    cs_deselect();
}





void SPI_code()
{
     // 1 MHz SPI (safe starting point; ST7735 can usually go much faster once this works)
    // spi_init(SPI_PORT, 10'000'000);
    spi_init(SPI_PORT, 1'000'000);
    // spi_init(SPI_PORT, 100'000);

    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    // MISO not used by ST7735, left unconfigured.

    // CS, DC, RST handled manually (bit-banged, not part of the SPI peripheral)
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);

    gpio_init(PIN_DC);
    gpio_set_dir(PIN_DC, GPIO_OUT);

    gpio_init(PIN_RST);
    gpio_set_dir(PIN_RST, GPIO_OUT);

    st7735_init();

    // Clear the whole screen to red as a minimal end-to-end test.
    const uint16_t RED_565 = 0xF800;
    fill_screen(RED_565);
}