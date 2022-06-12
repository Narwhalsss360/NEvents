#ifndef SecondsCounter_h
#define SecondsCounter_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include <NEvents.h>

struct TickEventArgs : public EventArgs
{
    uint32_t time;
    TickEventArgs()
        : EventArgs(), time(0)
    {
    }
    TickEventArgs(void *_sender, uint32_t _time)
        : EventArgs(_sender), time(_time)
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
    SecondsCounterClass() //Setting up ISR
        : seconds(0)
    {
        TCCR1A = ZERO;

        TCCR1B &= ~(1 << WGM13);
        TCCR1B |= (1 << WGM12);

        TCCR1B |= (1 << CS12);
        TCCR1B &= ~(1 << CS11);
        TCCR1B &= ~(1 << CS10);

        TCCR1A |= (1 << COM1A1) | (1 << COM1B1);

        TCNT1 = ZERO;
        OCR1A = 62500;
        TIMSK1 = (1 << OCIE1A);
        args.sender = this;
    }

    void tickISR()
    {
        seconds++;
        args.time = seconds;
        onTick.invoke(&args);
    }
};

extern SecondsCounterClass SecondsCounter;

#endif