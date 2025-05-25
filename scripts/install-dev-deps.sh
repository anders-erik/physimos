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


ninja
glfw
wayland
