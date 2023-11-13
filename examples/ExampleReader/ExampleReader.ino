#include <NEvents.h>

class PinReader
{
public:
    PinReader(const byte pin, const byte mode)
        : pin(pin)
    {
        pinMode(pin, mode);
    }

    void update()
    {
        bool currentState = digitalRead(pin);
        if (currentState != lastState)
        {
            lastState = currentState;
            pinChanged(pin, currentState);
        }
    }

    Event<PinReader, byte, bool> pinChanged;
private:
    const byte pin;
    bool lastState;
};

PinReader readers[3]
{
    PinReader(2, INPUT),
    PinReader(3, INPUT),
    PinReader(4, INPUT),
};

void pinChanged(byte pin, bool newState)
{
    Serial.print("Pin ");
    Serial.print((int)pin);
    Serial.print(" changed to ");
    Serial.println(newState);
}

void setup()
{
    Serial.begin(115200);
    readers[0].pinChanged += pinChanged;
    readers[1].pinChanged += pinChanged;
    readers[2].pinChanged += pinChanged;
}

void loop()
{
    for (int i = 0; i < 3; i++)
    {
        readers[i].update();
    }
}