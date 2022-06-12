#include <SecondsCounterExample.h>

/*
    User usage of your event driven library. See source code SecondsCounterExample.h and SecondsCounterExample.cpp.
*/

#define BAUDRATE 1000000

void tickEventHandler1(EventArgs *args)
{
    TickEventArgs data = *(TickEventArgs*)args;
    Serial.println("tickEventHandler1: " + String(data.seconds));
}

EVENT_FUNCTION(tickEventHandler2, args)
{
    TickEventArgs data = reinterpret_c_style(TickEventArgs, args);
    Serial.println("tickEventHandler2: " + String(data.seconds));
}

ESR(tickEventHandler3, args,
{
    TickEventArgs data = GET_TickEventArgs(args);
    Serial.println("tickEventHandler3: " + String(data.seconds));
})

TICK_ESR(tickEventHandler4, data,
{
    Serial.println("tickEventHandler4: " + String(data.seconds));
    Serial.println("\n\n\n");
})

void setup()
{
    Serial.begin(BAUDRATE);
    SecondsCounter.onTick += tickEventHandler1;
    SecondsCounter.onTickMuli += tickEventHandler2;
    SecondsCounter.onTickMuli += tickEventHandler3;
    SecondsCounter.onTickMuli += tickEventHandler4;
    SecondsCounter.timerSetup();
}

void loop()
{

}