#include "SecondsCounterExample.h"

SecondsCounterClass::SecondsCounterClass()
    : seconds(0), args({this, 0}), onTick(EventHandler()), onTickMuli(MultiFunctionEventHandler())
{
}

void SecondsCounterClass::timerSetup()
{
    TCCR1A = 0;

    TCCR1B &= ~(1 << WGM13);
    TCCR1B |= (1 << WGM12);

    TCCR1B |= (1 << CS12);
    TCCR1B &= ~(1 << CS11);
    TCCR1B &= ~(1 << CS10);

    TCCR1A |= (1 << COM1A1) | (1 << COM1B1);

    TCNT1 = 0;
    OCR1A = 62500;

    TIMSK1 |= (1 << OCIE1A);

    sei();
}

void SecondsCounterClass::tickISR()
{
    //Your useful code.
    seconds++;
    args.seconds = seconds;

    //Calls callbacks with &args.
    onTick.invoke(&args);
    onTickMuli.invoke(&args);
}

SecondsCounterClass SecondsCounter = SecondsCounterClass();

//ISR
ISR(TIMER1_COMPA_vect)
{
    SecondsCounter.tickISR();
}