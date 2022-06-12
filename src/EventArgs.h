#ifndef EventArgs_h
#define EventArgs_h

struct EventArgs
{
    void *sender;
    EventArgs();
    EventArgs(void *_sender);
};

#endif