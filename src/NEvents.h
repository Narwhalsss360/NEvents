#ifndef NEvents_h
#define NEvents_h

#include <Callables.h>

/// @brief Non-standard base class for event args
class EventArgs
{
public:
    EventArgs(void* context = nullptr);

    void* context;
};

template <typename Sender, typename... Args>
class Event
{
public:
    Event()
        : count(0), handlers(nullptr)
    {
    }
    
    /// @brief Adds an invokable to the event handler list.
    /// @param handler handler to add
    void addHandler(Invokable<void, Args...>* handler)
    {
        if (handler == nullptr) {
            return;
        }

        if (handlers == nullptr)
        {
            handlers = new Invokable<void, Args...>*[1];
            handlers[0] = handler;
            count++;
            return;
        }

        for (uint8_t i = 0; i < count; i++)
            if (handlers[i] == handler)
                return;

        Invokable<void, Args...>** newAllocation = new Invokable<void, Args...>*[count + 1];
        for (uint8_t i = 0; i < count; i++)
            newAllocation[i] = handlers[i];
        newAllocation[count] = handler;
        count++;

        delete[] handlers;
        handlers = newAllocation;
    }

    /// @brief Adds an invokable to the event handler list.
    /// @param handler handler to add
    void addHandler(Invokable<void, Args...>& handler)
    {
        addHandler(&handler);
    }

    /// @brief Removes the handler from the event handler list.
    /// @param handler handler to remove
    void removeHandler(Invokable<void, Args...>* handler)
    {
        int16_t index = -1;

        for (uint8_t i = 0; i < count; i++)
        {
            if (handlers[i] == handler)
            {
                index = i;
                break;
            }
        }
        
        if (index == -1)
            return;

        Invokable<void, Args...>** newAllocation = new Invokable<void, Args...>*[count - 1];

        for (uint8_t iDestination = 0, iSource = 0;
        iSource < count;
        iDestination++, iSource = (iSource == index -1 ? iSource + 2 : iSource + 1))
        {
            newAllocation[iDestination] = handlers[iSource];
        }

        count--;
        delete[] handlers;
        handlers = newAllocation;
    }

    /// @brief Removes the handler from the event handler list.
    /// @param handler handler to remove
    void removeHandler(Invokable<void, Args...>& handler)
    {
        removeHandler(&handler);
    }

    void operator+=(Invokable<void, Args...>* handler)
    {
        addHandler(handler);
    }

    void operator+=(Invokable<void, Args...>& handler)
    {
        addHandler(&handler);
    }

    void operator+=(void (*f)(Args...))
    {
        addHandler(InvokableVerifier::get(f));
    }

    void operator-=(Invokable<void, Args...>* handler)
    {
        removeHandler(handler);
    }

    void operator-=(Invokable<void, Args...>& handler)
    {
        removeHandler(&handler);
    }

    void operator-=(void (*f)(Args...))
    {
        removeHandler(InvokableVerifier::get(f));
    }

    ~Event()
    {
        if (handlers != nullptr)
            delete[] handlers;
        count = 0;
    }

private:
    friend Sender;

    /// @brief Invoke handlers.
    /// @param ...args args
    void invoke(Args... args) const
    {
        for (uint8_t i = 0; i < count; i++)
            if (handlers[i]->valid())
                handlers[i]->invoke(args...);
    }

    void invoke(Args... args)
    {
        ((const Event<Sender, Args...>*)this)->invoke(args...);
    }

    void operator()(Args... args) const
    {
        invoke(args...);
    }

    void operator()(Args... args)
    {
        invoke(args...);
    }

    uint8_t count;
    Invokable<void, Args...>** handlers;
};
#endif