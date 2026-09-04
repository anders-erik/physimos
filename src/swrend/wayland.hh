
#pragma once

#include <stdlib.h>
#include <sys/socket.h> // socket
#include <sys/un.h> // sockaddr_un

#include "lib/str.hh"


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


class WL
{
public:


};