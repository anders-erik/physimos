#!/bin/bash

ninja() {
  sudo apt install ninja-build
}
glfw(){
  
}
wayland(){
  sudo apt install libwayland-dev wayland-protocols
}


ninja
glfw
wayland
