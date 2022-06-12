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

#define NEVENTS_MAJOR_VERSION 1
#define NEVENTS_MINOR_VERSION 0
#define NEVENTS_PATCH_VERSION 1

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
typedef MultiFunctionEventHandler multi_event;

#endif