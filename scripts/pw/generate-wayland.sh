#! /bin/bash

function window_wayland()
{
    wayland_dir=src/window/wayland

    cd $wayland_dir

    mkdir -p protocols && cd protocols
    cp /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml .
    wayland-scanner client-header xdg-shell.xml xdg-shell-client-protocol.h
    wayland-scanner private-code xdg-shell.xml xdg-shell-protocol.c

}

function swrend_wayland()
{
    dest_dir=src/swrend/xdg_wl_extension
    mkdir -p $dest_dir && cd $dest_dir

    echo

    cp /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml .
    wayland-scanner client-header xdg-shell.xml xdg-shell-client-protocol.h
    wayland-scanner private-code xdg-shell.xml xdg-shell-protocol.c

    cd -
}

swrend_wayland
