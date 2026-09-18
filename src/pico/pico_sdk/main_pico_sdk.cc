// #include <cstdio>
// #include "pico/stdlib.h"

// int main()
// {
//     // stdio_init_all();

//     while (true) {
//         printf("Hello, world!\n");
//         sleep_ms(1000);
//     }
// }


#include "pico/stdlib.h"

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

    while (true)
    {
        v2_read_bool = gpio_get(V2_READ);
        v2_read_int = v2_read_bool ? 1 : 0;
        gpio_put(V3_WRITE, v2_read_int);

        if(v2_read_bool)
            gpio_put(V3_WRITE, 1);
        else
            gpio_put(V3_WRITE, 0);
        
        // gpio_put(V3_WRITE, 0);


        gpio_put(LED_0, 1);
        sleep_ms(1000);


        // v2_read_bool = gpio_get(V2_READ);
        // v2_read_int = v2_read_bool ? 1 : 0;
        // gpio_put(V3_WRITE, v2_read_int);


        v2_read_bool = gpio_get(V2_READ);
        v2_read_int = v2_read_bool ? 1 : 0;
        gpio_put(V3_WRITE, v2_read_int);

        if(v2_read_bool)
            gpio_put(V3_WRITE, 1);
        else
            gpio_put(V3_WRITE, 0);


        gpio_put(LED_0, 0);
        sleep_ms(1000);

        // gpio_put(V3_WRITE, 0);
    }

}
