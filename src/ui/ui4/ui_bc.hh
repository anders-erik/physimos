
#pragma once

#include "ui_node.hh"


enum class UIBCType
{
    f64,
} type;


/* UI Base Component */
template <typename T>
struct UIBC
{
    T* data = nullptr;
    UINode* node = nullptr;
    UIBCType type;

    UIBC() {}

    void set(T* _data, UINode* node)
    {
    }
};
