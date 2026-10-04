#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "tusb.h" // directory: $PHYSIMOS_ROOT_DIR/src/pico/pico_sdk/pico-sdk/lib/tinyusb/src/
// #include "hid.h"
// #include "hid_device.h"

#include "TFT_SPI_display.hh"



uint8_t keycode[6] = { 0 };
uint8_t modifier = 0;

// A "key up" report is pending from a previous press.
// 
static bool clear_pending = false;

bool v2_read_bool = false; // Read value at gpio_2
int v2_read_int = 0; // boolean conveted to int

const int LED_0 = 0;
bool LED_0_state = false;
const int V2_READ = 2;
const int V3_WRITE = 3;

bool button_0 = false;
static bool reported_button_0_state = false; // stores the most recently reported value
const int BUTTON_0_PIN = 6;
const int BUTTON_0_LED_PIN = 7;

bool button_1 = false;
static bool reported_button_1_state = false; // stores the most recently reported value
const int BUTTON_1_PIN_IN = 8;
const int BUTTON_1_PIN_OUT = 9;

bool button_2 = false;
static bool reported_button_2_state = false; // stores the most recently reported value
const int BUTTON_2_PIN_IN = 10;
const int BUTTON_2_PIN_OUT = 11;




struct KeyPressInput
{
    uint8_t modifier_ = 0;
    uint8_t keycode_[6];

    KeyPressInput(uint8_t _key_button_1, uint8_t _modifier)
        :   modifier_ {_modifier}
    {
        keycode_[0] = _key_button_1;
        keycode_[1] = 0;
        keycode_[2] = 0;
        keycode_[3] = 0;
        keycode_[4] = 0;
        keycode_[5] = 0;
    }  

    uint8_t get_modifier()
    {
        return modifier_;
    }

    uint8_t* get_key_code()
    {
        return keycode_;
    }

};

struct PhysicalButtonWithPin
{
    int PIN_READ;
    int PIN_WRITE;

    bool last_press_state = false;
    bool new_press = false;

    KeyPressInput press_input;

    PhysicalButtonWithPin(int _PIN_READ, int _PIN_WRITE, KeyPressInput _press_input)
        :   PIN_READ {_PIN_READ},
            PIN_WRITE {_PIN_WRITE},
            press_input {_press_input}
    {
        gpio_init(PIN_READ);
        gpio_set_dir(PIN_READ, GPIO_IN);
        gpio_init(PIN_WRITE);
        gpio_set_dir(PIN_WRITE, GPIO_OUT);
    }

    /** Outputs voltage when the input pin recieves voltage */
    void write_pin()
    {
        gpio_put(PIN_WRITE, read());
    }

    void write(bool _value)
    {
        gpio_put(PIN_WRITE, _value);
    }

    /**
        Checks wether the input pin is reading a voltage
    */
    bool read()
    {
        return gpio_get(PIN_READ);
    }


    /**
        Checks if a new press is detected.
        Stores the current read as the most recent one.
    */
    bool read_new_press()
    {
        bool new_press_detected = false;
        bool current_press_state = read();
        
        if(!last_press_state && current_press_state)
            new_press_detected = true;

        last_press_state = current_press_state;

        return new_press_detected;
    }

    uint8_t get_press()
    {
        return read_new_press() ? press_input.keycode_[0] : 0;
    }

    uint8_t* get_press_keycode()
    {
        return read_new_press() ? press_input.keycode_ : 0;
    }

    void set_keycode(uint8_t *_keycode)
    {
        if(read_new_press())
        {
            _keycode[0] = press_input.keycode_[0];
            _keycode[1] = press_input.keycode_[1];
            _keycode[2] = press_input.keycode_[2];
            _keycode[3] = press_input.keycode_[3];
            _keycode[4] = press_input.keycode_[4];
            _keycode[5] = press_input.keycode_[5];
        }
        else
        {
            _keycode[0] = 0;
            _keycode[1] = 0;
            _keycode[2] = 0;
            _keycode[3] = 0;
            _keycode[4] = 0;
            _keycode[5] = 0;
        }
    }

    uint8_t get_first_key()
    {
        return press_input.keycode_[0];
    }
};


unsigned long btn_3_modifier = KEYBOARD_MODIFIER_LEFTALT | KEYBOARD_MODIFIER_LEFTSHIFT;

PhysicalButtonWithPin physical_button_with_pin_3 {10, 11, {HID_KEY_B, btn_3_modifier}};


void init_GPIO()
{
    gpio_init(LED_0);
    gpio_set_dir(LED_0, GPIO_OUT);

    gpio_init(V2_READ);
    gpio_set_dir(V2_READ, GPIO_IN);
    gpio_init(V3_WRITE);
    gpio_set_dir(V3_WRITE, GPIO_OUT);


    gpio_init(BUTTON_1_PIN_IN);
    gpio_set_dir(BUTTON_1_PIN_IN, GPIO_IN);
    gpio_init(BUTTON_1_PIN_OUT);
    gpio_set_dir(BUTTON_1_PIN_OUT, GPIO_OUT);
}


void update_GPIO()
{
    v2_read_bool = gpio_get(V2_READ);
    v2_read_int = v2_read_bool ? 1 : 0;
    gpio_put(V3_WRITE, v2_read_int);


    bool blink = true;
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
    if (clear_pending)
    {
        tud_hid_keyboard_report(0, 0, NULL);
        clear_pending = false;
        return;
    }

    button_0 = gpio_get(BUTTON_0_PIN);
    gpio_put(BUTTON_0_LED_PIN, button_0);
    bool button_0_pressed_edge = button_0 && !reported_button_0_state;
    reported_button_0_state = button_0;

    button_1 = gpio_get(BUTTON_1_PIN_IN);
    gpio_put(BUTTON_1_PIN_OUT, button_1);
    bool button_1_pressed_edge = button_1 && !reported_button_1_state;
    reported_button_1_state = button_1;

    button_2 = gpio_get(BUTTON_2_PIN_IN);
    gpio_put(BUTTON_2_PIN_OUT, button_2);
    bool button_2_pressed_edge = button_2 && !reported_button_2_state;
    reported_button_2_state = button_2;

    if (button_0_pressed_edge)
    {
        keycode[0] = HID_KEY_SPACE;
        modifier = 0;
    }
    else if (button_1_pressed_edge)
    {
        keycode[0] = HID_KEY_A;
        // modifier = KEYBOARD_MODIFIER_LEFTCTRL;
        modifier = KEYBOARD_MODIFIER_LEFTALT | KEYBOARD_MODIFIER_LEFTSHIFT;
        // modifier = physical_button_with_pin_3.press_input.modifier_;
    }
    else if (button_2_pressed_edge)
    {
        keycode[0] = HID_KEY_B;
        modifier = 0;
    }
    else
    {
        return; // No new press this frame -- nothing to report.
    }

    tud_hid_keyboard_report(0, modifier, keycode);
    keycode[0] = 0;
    modifier = 0;
    clear_pending = true;

}   



int main()
{
    init_GPIO();
    tusb_init();
    stdio_init_all(); // ./src/pico/pico_sdk/pico-sdk/src/rp2_common/pico_stdio/include/pico/stdio.h

    SPI_code(); // will run all the display-code once

    sleep_us(1);

    while (true)
    {
        tud_task(); // If this is removed, the input on my computer does note work /AE, 2026-09-19 & 2026-09-26

        update_GPIO();

        // if(tud_hid_ready())
        report_usb();

        sleep_ms(10);

        printf("Booting...\n");
    }
}
