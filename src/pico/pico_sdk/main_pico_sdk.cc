#include "pico/stdlib.h"
#include "tusb.h" // directory: $PHYSIMOS_ROOT_DIR/src/pico/pico_sdk/pico-sdk/lib/tinyusb/src/
// #include "hid.h"
// #include "hid_device.h"


uint8_t keycode[6] = { 0 };
uint8_t modifier = 0;

bool v2_read_bool = false; // Read value at gpio_2
int v2_read_int = 0; // boolean conveted to int

const int LED_0 = 0;
bool LED_0_state = false;
const int V2_READ = 2;
const int V3_WRITE = 3;

bool spacebar = false;
static bool reported_space_state = false; // stores the most recently reported value
const int SPACEBAR_PIN = 6;
const int SPACEBAR_LED_PIN = 7;

bool build = false;
static bool reported_build_state = false; // stores the most recently reported value
const int BUILD_PIN_IN = 8;
const int BUILD_PIN_OUT = 9;





void init_GPIO()
{
    gpio_init(LED_0);
    gpio_set_dir(LED_0, GPIO_OUT);

    gpio_init(V2_READ);
    gpio_set_dir(V2_READ, GPIO_IN);
    gpio_init(V3_WRITE);
    gpio_set_dir(V3_WRITE, GPIO_OUT);

    gpio_init(SPACEBAR_PIN);
    gpio_set_dir(SPACEBAR_PIN, GPIO_IN);
    gpio_init(SPACEBAR_LED_PIN);
    gpio_set_dir(SPACEBAR_LED_PIN, GPIO_OUT);

    gpio_init(BUILD_PIN_IN);
    gpio_set_dir(BUILD_PIN_IN, GPIO_IN);
    gpio_init(BUILD_PIN_OUT);
    gpio_set_dir(BUILD_PIN_OUT, GPIO_OUT);
}

void update_GPIO()
{
    v2_read_bool = gpio_get(V2_READ);
    v2_read_int = v2_read_bool ? 1 : 0;
    gpio_put(V3_WRITE, v2_read_int);



    bool blink = false;
    if(blink)
    {
        LED_0_state = !LED_0_state;
        gpio_put(LED_0, LED_0_state);
    }
    else
    {
        gpio_put(LED_0, true);
    }

}


void report_usb()
{
    build = gpio_get(BUILD_PIN_IN);
    gpio_put(BUILD_PIN_OUT, build);
    if (build != reported_build_state)
    {
        keycode[0] = build ? HID_KEY_A : 0;
        reported_build_state = build;
    }

    spacebar = gpio_get(SPACEBAR_PIN);
    gpio_put(SPACEBAR_LED_PIN, spacebar);
    if (spacebar != reported_space_state)
    {
        keycode[1] =  spacebar ? HID_KEY_SPACE : 0;
        reported_space_state = spacebar;
    }

    tud_hid_keyboard_report(0, modifier, keycode); // ADD MODIFIERS!!!
}



int main()
{
    init_GPIO();
    tusb_init();


    while (true)
    {
        tud_task(); // If this is removed, the input on my computer does note work /AE, 2026-09-19 & 2026-09-26

        update_GPIO();

        if(tud_hid_ready())
            report_usb();

        sleep_ms(20);
    }
}
