# NEvents

## Files
  `Callback.h`: `struct Callback` declaration

  `Callback.cpp`: `struct Callback` defenition

  `EventArgs.h`: `struct EventArgs` declaration

  `EventArgs.cpp` `struct EventArgs` defention

  `NEvents.h`: `struct EventHandler` & `struct MultiFunctionEventHandler : public EventHandler` defenition

  `NEvents.cpp`: `struct EventHandler` & `struct MultiFunctionEventHandler : public EventHandler` declaration

  `SecondsCounterExample.h & .cpp`: Exmaple code for library creators.

## EventArgs
 Basic struct that contains a single `void * sender` which should be a pointer to the caller.

    struct EventArgs
    {
        void *sender;
        EventArgs();
        EventArgs(void *_sender);
    };


## Macros
  Contains macros for easier use of user and builder of a class.
  `EVENT_FUNCTION`: For basic declaratrion/defenition of a callback.

    #define EVENT_FUNCTION(name, args_name) void name(EventArgs *args_name)

  `EVENT_FUNCTION_PTR`: For basic declaration of a pointer to a function with the correct
  signature for an event callback.

    #define EVENT_FUNCTION_PTR(name) void (*name)(EventArgs *)

  `FUNCTION_PTR`: For basic using of any function that takes no arguments.

    #define FUNCTION_PTR(name) void (*name)(void)

  `ESR`: (Event Service Routine) For user usage to create a a callback function, example 
  code to create your ESR. should contain a reinterpret cast to autumatically
  case `EventArgs` to your type before user defenition of function.

    #define ESR(name, args_name, defenition) EVENT_FUNCTION(name, args_name) { defenition }

## Callback
  Simple struct just to contain a function pointer with `void (*name)(EventArgs *)` signature.

    struct Callback
    {
        EVENT_FUNCTION_PTR(function);
        Callback();
        Callback(EVENT_FUNCTION_PTR(_function));
    };

    Callback::Callback()
    : function(NULL)
    {
    }

    Callback::Callback(EVENT_FUNCTION_PTR(function))
        : function(function)
    {
    }

## EventHandler
  Struct object that can be used by a library builder in a class/struct to invoke callback
  functions contained in `Callback`. `EventHandler` struct is designed to only pass function
  pointers as arguments and the struct `Callback` container is completely internal to
  `EventHandler`.
  Create your own derived EventHandler for special usage, all member functions ae virtual.

  `typedef EventHandler event;`: For easier usage.

  `callback`: a `Callback *` that would contain a pointer to `Callback` to call a function in
  `callback` this is all internal to the `EventHandler` struct and your class shouldn't access
  this member for safety. `call = NULL` when user does not add a callback.

  `addCallback(EVENT_FUNCTION_PTR())`: A function to be used by user to add a function pointer
  with the signature:
  `void (*name)(EventArgs *)`.

  `invoke(EventArgs *)`: A function to be used by your class to invoke the callback with the
  arguments that your class provides.

  `operator+=(EVENT_FUNCTION_PTR())`: `operator` that simply calls `addCallback(EVENT_FUNCTION_PTR())` for easy usage.

    struct EventHandler
    {
        Callback *callback;
        EventHandler();
        virtual void addCallback(EVENT_FUNCTION_PTR());
        virtual void invoke(EventArgs *);
        virtual void operator+=(EVENT_FUNCTION_PTR());
    };

## MultiFunctionEventHandler
  Struct that derives from `EventHandler` and is used to handle an event with multiple callbacks.
  Usage of other functions would be the same as `EventHandler` with extra code to be able to add
  any number of callbacks defined by the user.

  `callbackCount`: The count of callbacks the user has added.

  `typedef MultiFunctionEventHandler multi_event;`: For easier usage.

    struct MultiFunctionEventHandler : public EventHandler
    {
        uint8_t callbackCount;
        MultiFunctionEventHandler();
        void addCallback(EVENT_FUNCTION_PTR()) override;
        void invoke(EventArgs *) override;
        virtual void operator+=(EVENT_FUNCTION_PTR());
    };

# SecondsCounter Example
  Example of your class in your library using `NEvents.h`
  You should create your own `ESR` & `GET_` for your own class for easier user use.
  This example is class that uses the ATmega328P's Timer1 to count seconds up accurately with
  callbacks using `EventHandler` & `MultiFunctionEventHandler` as samples.

  `GET_YourEventArgsDerivedTypeName` | `#define GET_TickEventArgs(arg)`: reinterpret cast
  macro to reinterpret `EventArgs` to `YourEventArgsDerivedTypeName`

  `YourClassName_ESR` | `TICK_ESR(name, args_name, defenition)`: Macro to make the creation
  and usage of a callback using your easier.

  .h file:

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

  .cpp File:

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

# User Usage(SecondsCounter Example)

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
    