#ifndef Callback_h
#define Callback_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include "EventArgs.h"
#include "NEventsMacros.h"

struct Callback
{
    EVENT_FUNCTION_PTR(function);
    Callback();
    Callback(EVENT_FUNCTION_PTR(_function));
};

#endif