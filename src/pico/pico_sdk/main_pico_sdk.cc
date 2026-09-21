#include "pico/stdlib.h"
#include "tusb.h" // directory: $PHYSIMOS_ROOT_DIR/src/pico/pico_sdk/pico-sdk/lib/tinyusb/src/
// #include "hid.h"
// #include "hid_device.h"


bool spacebar = false;
const int SPACEBAR_PIN = 6;
const int SPACEBAR_LED_PIN = 7;

bool build = false;
const int BUILD_PIN_IN = 8;
const int BUILD_PIN_OUT = 9;

void register_build_pins()
{
    // gpio_init(BUILD_PIN_IN);
    // gpio_set_dir(BUILD_PIN_OUT, GPIO_IN);
    // gpio_init(BUILD_PIN_IN);
    // gpio_set_dir(BUILD_PIN_OUT, GPIO_OUT);
}

// Whether the last HID report we sent already had space held down.
// Used to only send a new report when the pressed state actually changes.
static bool space_key_reported = false;

// Whether the last HID report we sent already had space held down.
// Used to only send a new report when the pressed state actually changes.
static bool build_key_reported = false;
    

// Send (or release) the HID space key based on the current `spacebar` value.
// Only sends a report when the state changes, and only once the host has
// finished enumerating the HID interface (tud_hid_ready()).
void update_spacebar_hid()
{
    if (!tud_hid_ready())
        return;

    if (spacebar == space_key_reported)
        return;

    uint8_t keycode[6] = { 0 };
    uint8_t modifier = 0;

    enum Output
    {
        SPACE,
        A,
        ALT_SHIFT_Z
    } output;

    // output = ALT_SHIFT_Z;
    // output = SPACE;
    output = A;

    if (spacebar)
    {
        switch (output)
        {
            case SPACE:
                keycode[0] = HID_KEY_SPACE;
                break;
            case A:
                keycode[0] = HID_KEY_A;
                break;
            case ALT_SHIFT_Z:
                modifier = KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_LEFTALT;
                keycode[0] = HID_KEY_Z;
                break;
        }
        
    }

    tud_hid_keyboard_report(0, modifier, keycode); // ADD MODIFIERS!!!
    space_key_reported = spacebar;
}



// Send (or release) the HID space key based on the current `spacebar` value.
// Only sends a report when the state changes, and only once the host has
// finished enumerating the HID interface (tud_hid_ready()).
void update_build_hid()
{
    if (!tud_hid_ready())
        return;

    if (build == build_key_reported)
        return;

    uint8_t keycode[6] = { 0 };
    uint8_t modifier = 0;

    enum Output
    {
        SPACE,
        A,
        ALT_SHIFT_Z
    } output;

    output = ALT_SHIFT_Z;
    // output = SPACE;
    // output = A;

    if (build)
    {
        switch (output)
        {
            case SPACE:
                keycode[0] = HID_KEY_SPACE;
                break;
            case A:
                keycode[0] = HID_KEY_A;
                break;
            case ALT_SHIFT_Z:
                modifier = KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_LEFTALT;
                keycode[0] = HID_KEY_Z;
                break;
        }
        
    }

    tud_hid_keyboard_report(0, modifier, keycode); // ADD MODIFIERS!!!
    build_key_reported = spacebar;
}

// Sleeps for `total_ms`, while still calling tud_task() and servicing the
// spacebar HID report every few milliseconds. Use this instead of sleep_ms()
// whenever the USB stack needs to stay responsive during a longer delay.
void delay_and_service_usb(uint32_t total_ms)
{
    const uint32_t step_ms = 10;
    for (uint32_t elapsed = 0; elapsed < total_ms; elapsed += step_ms)
    {
        tud_task(); // If this is removed, the input on my computer does note work /AE, 2026-09-19

        spacebar = gpio_get(SPACEBAR_PIN);
        gpio_put(SPACEBAR_LED_PIN, spacebar ? 1 : 0);
        // gpio_put(SPACEBAR_LED_PIN, spacebar ? 0 : 1);
        update_spacebar_hid();

        build = gpio_get(BUILD_PIN_IN);
        gpio_put(BUILD_PIN_OUT, build ? 1 : 0);
        // update_build_hid();

        sleep_ms(step_ms);
    }
}


int main() {
    const int LED_0 = 0;
    const int V2_READ = 2;
    const int V3_WRITE = 3;

    bool v2_read_bool = false; // Read value at gpio_2
    int v2_read_int = 0; // boolean conveted to int

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

    // register_build_pins();

    tusb_init();

    while (true)
    {

        v2_read_bool = gpio_get(V2_READ);
        v2_read_int = v2_read_bool ? 1 : 0;
        gpio_put(V3_WRITE, v2_read_int);

        gpio_put(LED_0, 1);

        spacebar = gpio_get(SPACEBAR_PIN);
        gpio_put(SPACEBAR_LED_PIN, spacebar ? 1 : 0);

        build = gpio_get(BUILD_PIN_IN);
        gpio_put(BUILD_PIN_OUT, build);
        // gpio_put(BUILD_PIN_OUT, build ? 1 : 0);

        // Spacebar HID report
        // tud_task();
        update_spacebar_hid();

        v2_read_bool = gpio_get(V2_READ);
        v2_read_int = v2_read_bool ? 1 : 0;
        gpio_put(V3_WRITE, v2_read_int);

        gpio_put(LED_0, 0);

        // update_build_hid();
        // delay_and_service_usb(1000);aa


        delay_and_service_usb(1000);
    }

}
