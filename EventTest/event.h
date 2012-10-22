#pragma once
#include "ptrsize.h"

#define EVENT_DEBUGGING

#ifdef EVENT_DEBUGGING
#define EVENT_DEBUG_INIT(name) name() \
	: function(__func__), file(__FILE__), line(__LINE__) \
	{ }
#else
#define EVENT_DEBUG_INIT(name)
#endif

#define MY_FCT __FUNCTION__

class EventDebugger;
class EventSystem;
class EventBase
{
public:
	friend EventDebugger;
	friend EventSystem;
	typedef PointerType<PointerSize::value>::value_type id_type;

	virtual ~EventBase() {};
	virtual id_type GetId() const = 0;
protected:
	EventBase() {}
#ifdef EVENT_DEBUGGING
	int dbg_line;
	const char* dbg_function;
	const char* dbg_file;
	//std::string dbg_uuid;
#endif
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