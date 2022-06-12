#ifndef Callback_h
#define Callback_h

#include "EventArgs.h"

struct Callback
{
    void (*function)(EventArgs *);
    Callback();
    Callback(void (*function)(EventArgs *));
};

#endif