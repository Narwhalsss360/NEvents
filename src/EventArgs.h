#ifndef EventArgs_h
#define EventArgs_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

struct EventArgs
{
    void *sender;
    EventArgs();
    EventArgs(void *_sender);
};

#endif