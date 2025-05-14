WAYLAND_FLAGS := $(shell pkg-config --cflags --libs wayland-client)

SRCS := main.c protocols/xdg-shell-protocol.c
BIN := wayland_c_window

all: $(BIN)

$(BIN): $(SRCS)
	$(CC) -std=c11 -fPIC -o $@ $(SRCS) $(WAYLAND_FLAGS)

clean:
	rm -f $(BIN)
