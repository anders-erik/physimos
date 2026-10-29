
#include "lib/print.hh"
#include "lib/clock.hh"

#include "math/vecmat.hh"

#include "audio/audio.hh"

#include "piano_app.hh"



struct Tester
{
    int a = 1;
    int b = 2;

    void hello()
    {
        Print::ln("Hello from tester");
    }
};

void (*tester_fn)(Tester*) = nullptr;

int main(int argc, char** argv)
{
    Print::ln("Hello from apps/piano/main_piano.cc! \n");

    UINode node;
    printf("sizeof(UINode) = %li \n", sizeof(UINode));
    printf("sizeof(Arr<int>) = %li \n", sizeof(Arr<int>));


    printf("sizeof(UI) = %li \n", sizeof(UI));

    Tester tester;
    tester.hello();
    // tester_fn = tester::hello;
    // tester_fn = Tester::hello;

    {
        Clock clock;
        clock.print_tv_start();

        // i2 win_dims = {640, 480};
        // i2 win_dims = {960, 720};
        i2 win_dims = {1500, 1000};

        PianoApp piano_app { win_dims };
        piano_app.open_window();

        Audio::play_note();
    }

    return 0;
}