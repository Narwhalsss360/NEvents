#include "NEvents.h"

EventHandler::EventHandler()
    : callback(NULL)
{
}

void EventHandler::addCallback(EVENT_FUNCTION_PTR(function))
{
    callback = (Callback *)malloc(sizeof(Callback));
    *callback = Callback(function);
}

void EventHandler::invoke(EventArgs *args)
{
    if (callback != NULL)
        callback->function(args);
}

void EventHandler::operator+=(EVENT_FUNCTION_PTR(function))
{
    addCallback(function);
}

MultiFunctionEventHandler::MultiFunctionEventHandler()
    : EventHandler(), callbackCount(ZERO)
{
}

void MultiFunctionEventHandler::addCallback(EVENT_FUNCTION_PTR(function))
{
    if (callbackCount == ZERO)
    {
        callback = (Callback *)malloc(sizeof(Callback));
        callback[callbackCount] = Callback(function);
        callbackCount++;
        return;
    }

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

void MultiFunctionEventHandler::operator+=(EVENT_FUNCTION_PTR(function))
{
    addCallback(function);
}