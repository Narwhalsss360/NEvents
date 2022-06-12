#include "Callback.h"

Callback::Callback()
    : function(NULL)
{
}

Callback::Callback(EVENT_FUNCTION_PTR(function))
    : function(function)
{
}