#ifndef NEvents_h
#define NEvents_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include <NDefs.h>
#include "Callback.h"

struct EventHandler
{
    Callback *callback;
    EventHandler();
    virtual void addCallback(void (*)(EventArgs *));
    virtual void invoke(EventArgs *);
    void operator+=(void (*)(EventArgs *));
};

struct MultiFunctionEventHandler : public EventHandler
{
    uint8_t callbackCount;
    MultiFunctionEventHandler();
    void addCallback(void (*)(EventArgs *)) override;
    void invoke(EventArgs *) override;
};

#endif