#define FUNCTION_PTR(name) void (*name)(void)
#define EVENT_FUNCTION(name, args_name) void name(EventArgs *args_name)
#define ESR(name, args_name, defenition) EVENT_FUNCTION(name, args_name) { defenition }
#define EVENT_LAMBDA(defenition) [](EventArgs *eventArgs) defenition
#define EVENT_FUNCTION_PTR(name) void (*name)(EventArgs *)