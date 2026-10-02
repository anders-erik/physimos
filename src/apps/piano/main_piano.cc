
#include "lib/print.hh"
#include "lib/clock.hh"

#include "math/vecmat.hh"

#include "audio/audio.hh"

#include "piano_app.hh"


int main(int argc, char** argv)
{
    Print::ln("Hello from apps/piano/main_piano.cc! \n");

    UINode node;
    printf("sizeof(UINode) = %d \n", sizeof(UINode));

    UINodeT<int> int_node;
    UINodeT<bool> bool_node;
    Arr<UINodeT<int>> int_node_arr;
    Arr<UINodeT<bool>> bool_node_arr;
    
    {
        Clock clock;
        clock.print_tv_start();

        i2 win_dims = {640, 480};

        PianoApp piano_app { win_dims };
        piano_app.open_window();

        Audio::play_note();
    }

    return 0;
}