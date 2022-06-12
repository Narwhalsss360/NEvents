#include "EventArgs.h"

EventArgs::EventArgs()
    : sender(NULL)
{
}

EventArgs::EventArgs(void *_sender)
    : sender(_sender)
{
}