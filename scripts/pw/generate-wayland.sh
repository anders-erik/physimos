#! /bin/bash

wayland_dir=src/window/wayland

cd $wayland_dir

mkdir -p protocols && cd protocols
cp /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml .
wayland-scanner client-header xdg-shell.xml xdg-shell-client-protocol.h
wayland-scanner private-code xdg-shell.xml xdg-shell-protocol.c

