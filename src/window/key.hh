#pragma once


enum class Key : unsigned char 
{
    None    = 0x00,

    shift           = 0x01,
    ctrl            = 0x02,
    ctrl_shift      = 0x03,
    alt             = 0x04,
    alt_shift       = 0x05,
    alt_ctrl        = 0x06,
    alt_ctrl_shift  = 0x07,
    Esc             = 0x08,

    caps    = 0x10, // treated as ctrl in glfw key code conversion

    F1      = 0x20,
    F2      = 0x21,
    F3      = 0x22,
    F4      = 0x23,
    F5      = 0x24,
    F6      = 0x25,
    F7      = 0x26,
    F8      = 0x27,
    F9      = 0x28,
    F10     = 0x29,
    F11     = 0x2A,
    F12     = 0x2B,

    a       = 0x61,
    b       = 0x62,
    c       = 0x63,
    d       = 0x64,
    e       = 0x65,
    f       = 0x66,
    g       = 0x67,
    h       = 0x68,
    i       = 0x69,
    j       = 0x6A,
    k       = 0x6B,
    l       = 0x6C,
    m       = 0x6D,
    n       = 0x6E,
    o       = 0x6F,
    p       = 0x70,
    q       = 0x71,
    r       = 0x72,
    s       = 0x73,
    t       = 0x74,
    u       = 0x75,
    v       = 0x76,
    w       = 0x77,
    x       = 0x78,
    y       = 0x79,
    z       = 0x7A,
};


