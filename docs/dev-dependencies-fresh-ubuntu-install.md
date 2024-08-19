
# Setup for running glthrowsim

## Installed using apt on 2024-08-17 (new Ubuntu 22.04 install)
```
apt git make [gcc] g++
```

- enabled me to build and run with two  s, otherwise everything OK.
    - I had to remove most g++ flags (only kept -lglfw3, which is included in project)
    - Top bar of window did not contain the usual hide, min/max, and close buttons


# apt packages to resolve broken window buttons


### apt-packages on laptop (2024-08-19)
https://medium.com/geekculture/a-beginners-guide-to-setup-opengl-in-linux-debian-2bfe02ccd1e

- mesa-common-dev
- libglu1-mesa (maybe included in libglu1-mesa-dev ??)

- libglfw3-dev (+ libglfw3-wayland)
- (libao4)

### Installing them one at a time on desktop

`sudo apt install mesa-common-dev` -> already the newest version

--- 

`sudo apt install libglfw3-dev` --> <br> 
The following NEW packages will be installed: <br>

- libegl-dev libegl1-mesa-dev libgles-dev libgles1 libglfw3 libglfw3-dev libglvnd-core-dev libglvnd-dev libopengl-dev libvulkan-dev


---

`sudo apt install libglu1-mesa-dev` <br> 
should contain: <br>
/usr/lib/x86_64-linux-gnu/libGLU.a <br>
/usr/lib/x86_64-linux-gnu/libGLU.so

## No package install resolved the issue

--- 
<br> <br>

# Resolving Lack of Window Decorations because Wayland does not do SSD 
<br>


### Original Problem:<br>
https://superuser.com/questions/1775370/wayland-compositor-seems-to-behave-differently-with-my-glfw-code-mainly-missing<br>

### Patch in GLFW using 'libdecor' in 3.3.9/3.3.10: <br>
https://github.com/glfw/glfw/issues/2212 <br> 
https://github.com/glfw/glfw/pull/2285 <br>

### New problem - not correct version through apt: <br>
```
$ apt list --installed | grep glfw

    libglfw3-dev/jammy,now 3.3.6-1 amd64 [installed]
    libglfw3/jammy,now 3.3.6-1 amd64 [installed,automatic]
```
--- 

### 1st attempted solution: Install GLFW 3.4 from source <br>
->  Still didn't work. Might be a problem with my installation, or that gcc is using the apt-installed version

### 2nd attempted solution: Include libdecor directly
- Uses Meson, which I've never used. Let's see if here is an already build package available

Found a dev package in debian repository
https://packages.debian.org/unstable/libdecor-0-dev
includes files:
```
/usr/include/libdecor-0/libdecor.h
/usr/lib/x86_64-linux-gnu/libdecor-0.so
```
which sounds good to me!

Let's install!

`sudo apt install libdecor-0-dev`

OK. Post install:

```
$ find /usr -name 'libdecor*'

    /usr/lib/x86_64-linux-gnu/libdecor
    /usr/lib/x86_64-linux-gnu/libdecor/plugins-1/libdecor-cairo.so
    /usr/lib/x86_64-linux-gnu/libdecor-0.so.0
    /usr/lib/x86_64-linux-gnu/libdecor-0.so
    /usr/lib/x86_64-linux-gnu/libdecor-0.so.0.100.0
    /usr/lib/x86_64-linux-gnu/pkgconfig/libdecor-0.pc
    /usr/include/libdecor-0
    /usr/include/libdecor-0/libdecor.h
    /usr/share/doc/libdecor-0-plugin-1-cairo
    /usr/share/doc/libdecor-0-dev
    /usr/share/doc/libdecor-0-0

```

IT WORKS ! ! ! ! ! !  ! !


Looking at the functioning leptop it had `libdecor-0-0` installed!

Still I don't know The interdependence of the packages that I've downloaded/installled. BUT  I do know that the above sequence WILL render the window decorations. 


### Heated discussion about SSD/CSR 
https://www.reddit.com/r/linux/comments/124sb4m/glfw_has_merged_proper_support_for_clientside/

> Client-side window decorations are when a program draws its own decorations (the titlebar, close/minimize buttons, etc) rather than them being drawn by the DE (that is called server-side decorations). <br><br>
GNOME infamously requires client-side decorations on Wayland (despite not doing so on X11). (Other DEs like KDE don't require this either.) Until GLFW merged this PR, it would just show a gray box around windows when running on GNOME/Wayland. As such, proper decorations are a massive improvement. <br><br>
This change will be included in GLFW 3.4 and possibly 3.3.9.<br><br> --- <br>
Note: CSD: This is client-side decorations & SSD: This is server-side decorations
