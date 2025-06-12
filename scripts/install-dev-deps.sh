#!/bin/bash

ninja() {
  sudo apt install ninja-build
}
glfw(){
    echo "no glfw installed"
}
wayland(){
  sudo apt install libwayland-dev wayland-protocols libdrm-dev
}
valgrind() {
  sudo apt install valgrind
}


ninja
glfw
wayland
valgrind