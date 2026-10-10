#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "tusb.h" // directory: $PHYSIMOS_ROOT_DIR/src/pico/pico_sdk/pico-sdk/lib/tinyusb/src/
// #include "hid.h"
// #include "hid_device.h"

#include "TFT_SPI_display.hh"

#define EnumCast() static_cast<uint>(Pin::read_2)

 // IO PINS          // Name printed next to physical display pins
// enum Pin
// {
//     POWER = 0,

//     BTN_TEST_IN = 2,
//     BTN_TEST_OUT = 3,

//     BTN_A_IN = 6,
//     BTN_A_OUT = 7,

//     BTN_B_IN = 8,
//     BTN_B_OUT = 9,

//     BTN_C_IN = 10,
//     BTN_C_OUT = 11,
// };


struct Pin
{
    // IO PINS          // Name printed next to physical display pins
    enum Num: unsigned int
    {
        POWER = 0,

        LIGHT_IN = 2,
        LIGHT_OUT = 3,

        A_IN = 6,
        A_OUT = 7,

        B_IN = 8,
        B_OUT = 9,

        C_IN = 10,
        C_OUT = 11,

        X_IN = 4,
        X_OUT = 5,

        Y_IN = 12,
        Y_OUT = 13,
    };
};

struct ResponsiveButton
{
    Pin::Num pin_in;
    Pin::Num pin_out;
    bool read = false;
    bool state = false;

    void update()
    {

    }
};


uint8_t keycode[6] = { 0 };
uint8_t modifier = 0;

// A "key up" report is pending from a previous press.
// 
static bool clear_pending = false;

// bool v2_read_bool = false; // Read value at gpio_2
// int v2_read_int = 0; // boolean conveted to int

ResponsiveButton RB_LIGHT = {   Pin::LIGHT_IN, 
                                Pin::LIGHT_OUT, 
                                false, 
                                false           };

ResponsiveButton RB_A       = { Pin::A_IN, 
                                Pin::A_OUT, 
                                false, 
                                false           };

ResponsiveButton RB_X       = { Pin::X_IN, 
                                Pin::X_OUT, 
                                false, 
                                false           };                                

ResponsiveButton RB_Y       = { Pin::Y_IN, 
                                Pin::Y_OUT, 
                                false, 
                                false       };

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
    gpio_init(Pin::POWER);
    gpio_set_dir(Pin::POWER, GPIO_OUT);

    gpio_init(RB_LIGHT.pin_in);
    gpio_set_dir(RB_LIGHT.pin_in, GPIO_IN);
    gpio_init(RB_LIGHT.pin_out);
    gpio_set_dir(RB_LIGHT.pin_out, GPIO_OUT);


    // gpio_init(Pin::DRAW_IN);
    // gpio_set_dir(Pin::DRAW_IN, GPIO_IN);
    // gpio_init(Pin::DRAW_OUT);
    // gpio_set_dir(Pin::DRAW_OUT, GPIO_OUT);

    gpio_init(RB_A.pin_in);
    gpio_set_dir(RB_A.pin_in, GPIO_IN);
    gpio_init(RB_A.pin_out);
    gpio_set_dir(RB_A.pin_out, GPIO_OUT);

    gpio_init(BUTTON_1_PIN_IN);
    gpio_set_dir(BUTTON_1_PIN_IN, GPIO_IN);
    gpio_init(BUTTON_1_PIN_OUT);
    gpio_set_dir(BUTTON_1_PIN_OUT, GPIO_OUT);


    gpio_init(RB_X.pin_in);
    gpio_set_dir(RB_X.pin_in, GPIO_IN);
    gpio_init(RB_X.pin_out);
    gpio_set_dir(RB_X.pin_out, GPIO_OUT);

    gpio_init(RB_Y.pin_in);
    gpio_set_dir(RB_Y.pin_in, GPIO_IN);
    gpio_init(RB_Y.pin_out);
    gpio_set_dir(RB_Y.pin_out, GPIO_OUT);
}


void update_GPIO()
{
    gpio_put(Pin::POWER, true);

    // gpio_put(RB_LIGHT.read, true);

    RB_LIGHT.read = gpio_get(RB_LIGHT.pin_in);
    RB_LIGHT.state = RB_LIGHT.read ? 1 : 0;
    gpio_put(RB_LIGHT.pin_out, RB_LIGHT.state);

    // bool blink = true;
    // if(blink)
    // {
    //     RB_TEST.state = !RB_TEST.state;
    //     gpio_put(RB_TEST.read, RB_TEST.state);
    // }
    // else
    // {
    //     gpio_put(RB_TEST.read, true);
    // }

    RB_X.read = gpio_get(Pin::X_IN);
    RB_X.state = RB_X.read ? 1 : 0;
    gpio_put(RB_X.pin_out, RB_X.state);

    RB_Y.read = gpio_get(RB_Y.pin_in);
    RB_Y.state = RB_Y.read ? 1 : 0;
    gpio_put(RB_Y.pin_out, RB_Y.state);
}


void report_usb()
{
    if (clear_pending)
    {
        tud_hid_keyboard_report(0, 0, NULL);
        clear_pending = false;
        return;
    }

    RB_A.read = gpio_get(RB_A.pin_in);
    gpio_put(RB_A.pin_out, RB_A.read);
    bool button_0_pressed_edge = RB_A.read && !RB_A.state;
    RB_A.state = RB_A.read;

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
    // SPI_set_black();
    // SPI_set_white();
    SPI_set_green();
    // pixel_set(10, 10, 10, 10, 0x0000);
    SPI_set_black_square(10, 10, 10, 10);

    sleep_us(1);

    while (true)
    {
        tud_task(); // If this is removed, the input on my computer does note work /AE, 2026-09-19 & 2026-09-26

        update_GPIO();

        if(RB_X.read)
            SPI_set_black_square(10, 10, 10, 10);
        else
            SPI_set_green_square(10, 10, 10, 10);
        
        if(RB_Y.read)
            SPI_set_white_square(20, 20, 10, 10);
        else
            SPI_set_green_square(20, 20, 10, 10);

        // if(tud_hid_ready())
        report_usb();

        sleep_ms(10);

        printf("Booting...\n");
    }
}
