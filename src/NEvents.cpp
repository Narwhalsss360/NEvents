#include "NEvents.h"

EventHandler::EventHandler()
    :callback(NULL)
{
}

virtual void EventHandler::addCallback(void (*function)(EventArgs *))
{
    *callback = Callback(function);
}

virtual void EventHandler::invoke(EventArgs *args)
{

}

void EventHandler::operator+=(void (*function)(EventArgs *))
{
    addCallback(function);
}