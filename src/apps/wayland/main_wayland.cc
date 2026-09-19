
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h> // socket
#include <sys/un.h> // sockaddr_un

#include "lib/arr.hh"
#include "lib/bitmap.hh"
#include "lib/cli.hh"
#include "lib/clock.hh"
#include "lib/file.hh"
#include "lib/print.hh"
#include "lib/str.hh"

#include "math/vec.hh"
#include "math/vecmat.hh"
#include "math/line.hh"

#include "audio/audio.hh"
#include "window/wayland1/wayland.hh"
#include "ui/ui4/ui.hh"

// #include "piano_app.hh"
#include "io/bmp2/BMP.hh"
#include "rend/sw/swrend.hh"



struct XDG
{
    static Str get_runtime_dir()
    {
        const char *runtime = getenv("XDG_RUNTIME_DIR");
        return Str{runtime};
    };

    static Str get_wayland_display_socket_name()
    {
        const char *wayland_display = getenv("WAYLAND_DISPLAY");
        return Str{wayland_display};
    };
};


struct Socket
{
    static Str read_from_socket(Str socket_path)
    {
        int fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (fd < 0)
            return "ERROR: failed to open socket.";
        
        // int flags = fcntl(fd, F_GETFL, 0);
        // fcntl(fd, F_SETFL, flags | O_NONBLOCK);

        struct sockaddr_un addr = {0};
        addr.sun_family = AF_UNIX;

        strncpy(addr.sun_path, socket_path.to_c_str(), sizeof(addr.sun_path) - 1);

        if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            close(fd);
            return "Connected to socket with error code.";
        }

        // Wayland socket is available. Server will wait for initiation of wl-protocal mesage before providing data.

        char buf[4096];

        ssize_t n;
        // ssize_t n = read(fd, buf, sizeof(buf));
        return "Early return because 'read' blocks the process and no wayland connection is available.";

        if (n < 0) {
            perror("read");
        } else if (n == 0) {
            printf("server disconnected\n");
        } else {
            printf("received %zd bytes\n", n);
        }

        close(fd);

        return "Connected to socket.";
    };
};


void socket_test()
{
    Print::buf("\n");

    Str xdg_rt_dir = XDG::get_runtime_dir();
    Print::buf("xdg_rt_dir = ");
    Print::ln(xdg_rt_dir);

    Str xdg_wayland_socket = XDG::get_wayland_display_socket_name();
    Print::buf("xdg_wayland_socket = ");
    Print::ln(xdg_wayland_socket);

    Str wl_socket_path = xdg_rt_dir + "/" + xdg_wayland_socket;
    Print::buf("wl_socket_path = ");
    Print::ln(wl_socket_path);

    Str wl_socket_read = Socket::read_from_socket(wl_socket_path);
    Print::buf("wl_socket_read = ");
    Print::ln(wl_socket_read);
}



int main(int argc, char** argv)
{
    Print::ln("Hello from main_wayland.cc");

    CLI cli {argc, argv};

    // socket_test();
    
    Wayland wayland;

    // if(wayland.setup_ok)
    //     wayland.run();
    while(wayland.frame_step())
    {
        usleep(10000);
    }
    wayland.close();

    // Wayland wayland2;
    // wayland2.run();
    

    Print::ln("Bye, from main_wayland.cc. \n");
    return 0;
}