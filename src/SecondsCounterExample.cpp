#include "SecondsCounterExample.h"

SecondsCounterClass SecondsCounter = SecondsCounterClass();

ISR(TIMER1_COMPA_vect)
{
    SecondsCounter.tickISR();
}