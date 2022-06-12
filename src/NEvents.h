#ifndef NEvents_h
#define NEvents_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include <NDefs.h>
#include "EventArgs.h"
#include "Callback.h"
#include "NEventsMacros.h"

struct EventHandler
{
    Callback *callback;
    EventHandler();
    virtual void addCallback(EVENT_FUNCTION_PTR());
    virtual void invoke(EventArgs *);
    virtual void operator+=(EVENT_FUNCTION_PTR());
};

struct MultiFunctionEventHandler : public EventHandler
{
    uint8_t callbackCount;
    MultiFunctionEventHandler();
    void addCallback(EVENT_FUNCTION_PTR()) override;
    void invoke(EventArgs *) override;
    virtual void operator+=(EVENT_FUNCTION_PTR());
};

typedef EventHandler event;

#endif