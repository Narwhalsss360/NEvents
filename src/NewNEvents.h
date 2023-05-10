#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include <DynamicArray.h>

struct EventArgs
{
    void* sender;
};

class Callable;
class EventHandler;

class Callable
{
    bool defaulted;
    void (*f)(EventArgs);
    friend class EventHandler;
    void invoke(EventArgs);
    virtual void invoked(EventArgs);
public:
    bool wasDefaulted();
    Callable();
    Callable(bool);
    Callable(void (*f)(EventArgs));

    bool operator==(Callable& other)
    {
        if (defaulted != other.defaulted) return false;
        if (f != other.f) return false;
        if (&other != this) return false;
        return true;
    }
};

void Callable::invoke(EventArgs args)
{
    if (f) f(args);
    else invoked(args);
}

void Callable::invoked(EventArgs args)
{
}

bool Callable::wasDefaulted()
{
    return defaulted;
}

Callable::Callable()
    : Callable(true)
{
}

Callable::Callable(bool defaulted = true)
    : defaulted(defaulted), f(nullptr)
{
}

Callable::Callable(void (*f)(EventArgs))
    : defaulted(false), f(f)
{
}

template <typename ClassWithEvents>
class EventHandler
{
private:
    DynamicArray<Callable&> callables;
    friend class ClassWithEvents;
protected:
    void invoke(EventArgs);
public:
    EventHandler();
    void addHandler(Callable& callable);
    void removeHandler(Callable& callable);
    ~EventHandler();
};

EventHandler::EventHandler()
    : callables(DynamicArray<Callable>())
{
}

void EventHandler::invoke(EventArgs args)
{
    for (Callable& callable : callables) callable.invoke(args);
}

void EventHandler::addHandler(Callable& callable)
{
    if (callables.has(callable)) return;
    callabes.append(callable);
}

void EventHandler::removeHandler(Callable& callable)
{
    if (!callables.has(callable)) return;
    callables.remove(callables.indexOf(callable));
}

EventHandler::~EventHandler()
{
}

class PinStateChanged : Callable
{
protected:
    void invoked(EventArgs) { pinStateChangedEventHandler(args); }
    virtual void pinStateChangedEventHandler(EventArgs) { }
public:
    PinStateChanged()
        : Callable(false)
    {
    }
};

class PinStateChangeReader
{
public:
    EventHandler<PinStateChangeReader> PinStateChangedEvent;
    byte oldP;

    PinStateChangeReader()
        : PinStateChangedEvent(EventHandler<PinStateChangedReader>()), oldP(false)
    {
    }

    void update(uint8_t p)
    {
        if (oldP != digitalRead(p))
        {
            oldP = digitalRead(p);
            EventArgs args;
            args.sender = this;
            PinStateChangedEvent.invoke(args);
        }
    }
};

class PinChangedNotifier : PinStateChanged
{
    PinChangedNotifier()
        : PinStateChanged()
    {
    }

    void pinStateChangedEventHandler(EventArgs args)
    {
        Serial.println("Pin State Changed");
    }
};

PinStateChangeReader pscr = PinStateChangeReader();
PinChangedNotifier pcn = PinChangedNotifier();

void setup()
{
    Serial.begin(115200);
    pscr.PinStateChangedEvent.addHandler(pcn);
}

void loop()
{
    pscr.update(3);
}