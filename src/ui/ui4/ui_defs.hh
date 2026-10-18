#pragma once

#define UI_CALLBACK_CAST (UIEventReturn (*)(UINode*, UIEvent, void*))    // enable the UI to dispatch any data using void*
#define UI_CALLBACK_PARAMS(x) UINode* node, UIEvent ui_event, x* data // useful for API changes!

#define UIC__ (template<typename T> struct UIC)


/** Planned return type for a particular component 
    **NOT YET IMPLEMENTED** */
template <typename A, typename C>
struct UIComponentContext
{
    UI* ui;
    A*  app;
    C*  component;
};

/*  To be returned in every UI Node event callback. 
    Different type for each specific app. */
template <typename T>
struct UIAppContext
{
    UI* ui;
    T*  app;
};