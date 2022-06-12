#include "NEvents.h"

EventHandler::EventHandler()
    : callback(NULL)
{
}

virtual void EventHandler::addCallback(void (*function)(EventArgs *))
{
    *callback = Callback(function);
}

virtual void EventHandler::invoke(EventArgs *args)
{
    callback->function(args);
}

void EventHandler::operator+=(void (*function)(EventArgs *))
{
    addCallback(function);
}

MultiFunctionEventHandler::MultiFunctionEventHandler()
    : EventHandler(), callbackCount(ZERO)
{
}

void MultiFunctionEventHandler::addCallback(void (*function)(EventArgs *)) override
{
    Callback *temp = (Callback *)malloc(sizeof(Callback) * (callbackCount + 1));
    memmove(temp, callback, sizeof(Callback) * callbackCount);

    callback = (Callback *)malloc(sizeof(Callback) * (callbackCount + 1));
    memmove(callback, temp, sizeof(Callback) * (callbackCount + 1));
    callback[callbackCount] = Callback(function);

    callbackCount++;
    free(temp);
}

void MultiFunctionEventHandler::invoke(EventArgs *args)
{
    for (uint8_t callbackIndex = ZERO; callbackIndex < callbackCount; callbackIndex++)
    {
        callback[callbackIndex].function(args);
    }
}