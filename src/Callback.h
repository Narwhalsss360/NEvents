#ifndef Callback_h
#define Callback_h

#include "EventArgs.h"
#include "Macros.h"

struct Callback
{
    EVENT_FUNCTION_PTR(function);
    Callback();
    Callback(EVENT_FUNCTION_PTR(_function));
};

#endif