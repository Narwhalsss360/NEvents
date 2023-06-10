#ifndef NEvents_h
#define NEvents_h

#include <Callables.h>

template <class Owner>
class Event
{
public:
	Event(VoidCallableAny* callable = nullptr)
		: callable(callable)
	{
	}

	void addHandler(VoidCallableAny* callable)
	{
		this->callable = callable;
	}

	void operator+=(VoidCallableAny& callable)
	{
		addHandler(&callable);
	}

private:
	void invoke(any_ptr pointer = nullptr)
	{
		if (callable)
			callable->invoke(pointer);
	}

	void operator()(any_ptr pointer = nullptr)
	{
		invoke(pointer);
	}

	VoidCallableAny* callable;

	friend Owner;
};
#endif