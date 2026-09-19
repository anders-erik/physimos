#include "pico/stdlib.h"
#include "tusb.h" // directory: $PHYSIMOS_ROOT_DIR/src/pico/pico_sdk/pico-sdk/lib/tinyusb/src/
// #include "hid.h"
// #include "hid_device.h"

bool spacebar = false;
const int SPACEBAR_PIN = 6;
const int SPACEBAR_LED_PIN = 7;

// Whether the last HID report we sent already had space held down.
// Used to only send a new report when the pressed state actually changes.
static bool space_key_reported = false;
    

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
        ALT_SHIFT_Z
    } output;

    // output = ALT_SHIFT_Z;
    output = SPACE;

    if (spacebar)
    {
        switch (output)
        {
            case SPACE:
                keycode[0] = HID_KEY_SPACE;
                break;
            case ALT_SHIFT_Z:
                modifier = KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_LEFTALT;
                keycode[0] = HID_KEY_Z;
                break;
        }
        
    }

    tud_hid_keyboard_report(0, modifier, keycode);
    space_key_reported = spacebar;
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
        update_spacebar_hid();

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

    tusb_init();

    while (true)
    {

        v2_read_bool = gpio_get(V2_READ);
        v2_read_int = v2_read_bool ? 1 : 0;
        gpio_put(V3_WRITE, v2_read_int);

        gpio_put(LED_0, 1);

        spacebar = gpio_get(SPACEBAR_PIN);
        gpio_put(SPACEBAR_LED_PIN, spacebar ? 1 : 0);

        // Spacebar HID report
        // tud_task();
        update_spacebar_hid();
        delay_and_service_usb(1000);

        v2_read_bool = gpio_get(V2_READ);
        v2_read_int = v2_read_bool ? 1 : 0;
        gpio_put(V3_WRITE, v2_read_int);

        gpio_put(LED_0, 0);

        delay_and_service_usb(1000);
    }

}
