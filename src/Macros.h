
#define EVENT_FUNCTION(name, args_name) void name(EventArgs *args_name)
#define EVENT_FUNCTION_PTR(name) void (*name)(EventArgs *)
#define FUNCTION_PTR(name) void (*name)(void)