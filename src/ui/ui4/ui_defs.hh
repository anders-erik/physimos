#pragma once

#define UI_CALLBACK_CAST (UIEventReturn (*)(UINode*, UIEvent, void*))    // enable the UI to dispatch any data using void*
#define UI_CALLBACK_PARAMS(x) UINode* node, UIEvent ui_event, x* data // useful for API changes!

#define UIC__ (template<typename T> struct UIC)


template <typename A, typename C>
struct UIContext
{
    UI* ui;
    A*  app;
    C*  component;
};