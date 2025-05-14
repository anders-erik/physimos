#! /bin/bash

cd src/window/wayland

mkdir -p protocols && cd protocols
cp /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml .
wayland-scanner client-header xdg-shell.xml xdg-shell-client-protocol.h
wayland-scanner private-code xdg-shell.xml xdg-shell-protocol.c

