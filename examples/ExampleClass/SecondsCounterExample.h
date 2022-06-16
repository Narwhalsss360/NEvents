#ifndef SecondsCounter_h
#define SecondsCounter_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include <NEvents.h>

#define GET_TickEventArgs(arg) *(TickEventArgs*)arg
#define TICK_ESR(name, args_name, defenition)  EVENT_FUNCTION(name, base_##args_name) {  TickEventArgs args_name = GET_TickEventArgs(base_##args_name); defenition }

// Your data struct thats passed as an arg to event callback.
// sender in EventArgs should be a pointer to the object that called the callback.
struct TickEventArgs : public EventArgs
{
    uint32_t seconds;
    TickEventArgs()
        : EventArgs(), seconds(0)
    {
    }
    TickEventArgs(void *_sender, uint32_t _time)
        : EventArgs(_sender), seconds(_time)
    {
    }
};

//Your useful class.
class SecondsCounterClass
{
private:
    uint32_t seconds;
    TickEventArgs args; //Argument thats passed to callback.
public:
    EventHandler onTick; //EventHandler
    MultiFunctionEventHandler onTickMuli; //EventHandler
    SecondsCounterClass();
    void timerSetup(); //Sets up ISR for second counting.
    void tickISR(); //Called by ISR, not user.
};

extern SecondsCounterClass SecondsCounter;

#endif