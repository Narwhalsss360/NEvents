#ifndef SecondsCounter_h
#define SecondsCounter_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include <NEvents.h>

#define GET_TickEventArgs(arg) *(TickEventArgs*)arg
#define TICK_ESR(name, args_name, defenition) void name(EventArgs *base_##args_name) {  TickEventArgs args_name = GET_TickEventArgs(base_##args_name); defenition }

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

class SecondsCounterClass
{
private:
    uint32_t seconds;
    TickEventArgs args;
public:
    EventHandler onTick;
    MultiFunctionEventHandler onTickMuli;
    SecondsCounterClass();
    void timerSetup();
    void tickISR();
};

extern SecondsCounterClass SecondsCounter;

#endif