
#include "lcdspi.h"
#include "hardware/spi.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"

static uint32_t irq_state;
static bool lcd_initialised = false;

static uint16_t frame_buffer[WIDTH*HEIGHT];

static void lcd_disable_interrupts()
{
    irq_state = save_and_disable_interrupts();
    //gpio_put(3, true);
}

static void lcd_enable_interrupts()
{
    //gpio_put(3, false);
    restore_interrupts(irq_state);
}


// Helper to send a single command byte
void display_send_cmd(uint8_t cmd) {
    gpio_put(LCD_DC, 0); // Command Mode
    gpio_put(LCD_CS, 0);
    spi_write_blocking(SPI_PORT, &cmd, 1);
    gpio_put(LCD_CS, 1);
}

// Helper to send data bytes
void display_send_data(uint8_t len, ...)
{
    va_list args;
    va_start(args, len);
    gpio_put(LCD_DC, 1); // Data
    gpio_put(LCD_CS, 0);
    for (uint8_t i = 0; i < len; i++)
    {
        uint8_t data = va_arg(args, int); // get the next byte of data
        spi_write_blocking(SPI_PORT, &data, 1);
    }
    gpio_put(LCD_CS, 1);
    va_end(args);
}


// Turn on the LCD display
void lcd_display_on()
{
    lcd_disable_interrupts();
    display_send_cmd(LCD_CMD_DISPON);
    lcd_enable_interrupts();
}

// Turn off the LCD display
void lcd_display_off()
{
    lcd_disable_interrupts();
    display_send_cmd(LCD_CMD_DISPOFF);
    lcd_enable_interrupts();
}

void display_reset()
{
    // Blip the reset pin to reset the LCD controller
    gpio_put(LCD_RST, 0);
    busy_wait_us(20); // 20µs reset pulse (10µs minimum)

    gpio_put(LCD_RST, 1);
    busy_wait_us(120000); // 5ms required after reset, but 120ms needed before sleep out command
}

void lcd_write16_buf(const uint16_t *buffer, size_t len)
{
    // DO NOT MOVE THE spi_set_format() OR THE gpio_put(LCD_DCX) CALLS!
    // They are placed before the gpio_put(LCD_CSX) to ensure that a minimum
    // chip select high pulse width is achieved (at least 40ns)
    spi_set_format(SPI_PORT, 16, 0, 0, SPI_MSB_FIRST);

    gpio_put(LCD_DC, 1); // Data
    gpio_put(LCD_CS, 0);
    spi_write16_blocking(SPI_PORT, buffer, len);
    gpio_put(LCD_CS, 1);

    spi_set_format(SPI_PORT, 8, 0, 0, SPI_MSB_FIRST);
}

static void lcd_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    // Set column address (X)
    display_send_cmd(LCD_CMD_CASET);
    display_send_data(4,
                   UPPER8(x0), LOWER8(x0),
                   UPPER8(x1), LOWER8(x1));

    // Set row address (Y)
    display_send_cmd(LCD_CMD_RASET);
    display_send_data(4,
                   UPPER8(y0), LOWER8(y0),
                   UPPER8(y1), LOWER8(y1));

    // Prepare to write to RAM
    display_send_cmd(LCD_CMD_RAMWR);
}

void lcd_blit(const uint16_t *pixels, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    lcd_disable_interrupts();

    lcd_set_window(x, y, x + width - 1, y + height - 1);

    lcd_write16_buf((uint16_t *)pixels, width * height);

    lcd_enable_interrupts();
}


void display_solid_rectangle(uint16_t colour, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {

    for (uint16_t y_pos = 0; y_pos < height; y_pos++)
    {
        for (uint16_t x_pos = 0; x_pos < width; x_pos++)
        {
            frame_buffer[x_pos + y_pos*width] = colour;
        }
    }
    lcd_blit(frame_buffer, x, y, width, height);
}

void display_clear_screen() {
    display_solid_rectangle(0x00, 0, 0, WIDTH, HEIGHT);
}


void display_init()
{
    if (lcd_initialised)
    {
        return; // already initialized
    }

    // initialise GPIO
    gpio_init(LCD_SCK);
    gpio_init(LCD_TX);
    gpio_init(LCD_RX);
    gpio_init(LCD_CS);
    gpio_init(LCD_DC);
    gpio_init(LCD_RST);

    gpio_set_dir(LCD_SCK, GPIO_OUT);
    gpio_set_dir(LCD_TX, GPIO_OUT);
    gpio_set_dir(LCD_CS, GPIO_OUT);
    gpio_set_dir(LCD_DC, GPIO_OUT);
    gpio_set_dir(LCD_RST, GPIO_OUT);

    // initialise 4-wire SPI
    spi_init(SPI_PORT, LCD_BAUDRATE);
    gpio_set_function(LCD_SCK, GPIO_FUNC_SPI);
    gpio_set_function(LCD_TX, GPIO_FUNC_SPI);
    gpio_set_function(LCD_RX, GPIO_FUNC_SPI);

    gpio_put(LCD_CS, 1);
    gpio_put(LCD_RST, 1);

    lcd_disable_interrupts();

    display_reset(); // reset the LCD controller

    display_send_cmd(LCD_CMD_SWRESET); // reset the commands and parameters to their S/W Reset default values
    busy_wait_us(10000);                   // required to wait at least 5ms

    display_send_cmd(LCD_CMD_COLMOD); // pixel format set
    display_send_data(1, 0x55);       // 16 bit/pixel (RGB565)

    display_send_cmd(LCD_CMD_MADCTL); // memory access control
    display_send_data(1, 0x48);       // BGR colour filter panel, top to bottom, left to right

    display_send_cmd(LCD_CMD_INVON); // display inversion on

    display_send_cmd(LCD_CMD_EMS); // entry mode set
    display_send_data(1, 0xC6);    // normal display, 16-bit (RGB) to 18-bit (rgb) colour
                                //   conversion: r(0) = b(0) = G(0)

    display_send_cmd(LCD_CMD_VSCRDEF); // vertical scroll definition
    display_send_data(6,
                   0x00, 0x00, // top fixed area of 0 pixels
                   0x01, 0x40, // scroll area height of 320 pixels
                   0x00, 0x00  // bottom fixed area of 0 pixels
    );

    display_send_cmd(LCD_CMD_SLPOUT); // sleep out
    lcd_enable_interrupts();

    busy_wait_us(10000);                  // required to wait at least 5ms

    // Clear the screen
    display_clear_screen();

    // Now that the display is initialized, display RAM garbage is cleared,
    // turn on the display
    lcd_display_on();

    lcd_initialised = true; // Set the initialised flag
}