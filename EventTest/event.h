#pragma once

#include "ptrsize.h"

namespace es {

class EventBase
{
public:
	typedef PointerType<PointerSize::value>::value_type id_type;

	virtual ~EventBase() {};
	virtual id_type GetId() const = 0;
protected:
	EventBase() {}
};

template<typename T>
class Event : public EventBase
{
public:
	virtual ~Event() {}
	id_type GetId() const
	{
		return id;
	}
protected:    
	Event()
		: id(s_id())
	{ }
private:
	static id_type s_id()
	{
		static int val = 5;
		return reinterpret_cast<id_type>(&val);
	}

	id_type id;
};

class NullEvent : public Event<NullEvent>
{

};

/*
// null event alternatvive, has a statically defined it
struct NullEvent : public EventBase
{
	// lets assume that no sane system offers 0 as valid pointer position
	static const int id = 0x0;

	EventBase::id_type GetId() const
	{
		return id;
	}
};*/

}; // namespace