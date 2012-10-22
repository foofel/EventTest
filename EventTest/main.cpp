#include "event.h"
#include "eventsystem.h"
#include "eventdebuger.h"
#include <string>
#include <chrono>
#include <iostream>

template<typename T>
class MyEventBase : public Event<T>
{
public:
	MyEventBase()
	{
	}
	virtual ~MyEventBase()
	{

	}
	std::string GetData() const
	{
		return data;
	}
	std::string data;
};

class KeyboardEvent : public MyEventBase<KeyboardEvent>
{
public:
	KeyboardEvent()
	{
	}

	KeyboardEvent(const std::string &msg)
	{
		data = msg;
	}
};

class MouseEvent : public MyEventBase<MouseEvent>
{
public:
	MouseEvent()
	{
	}

	MouseEvent(const std::string &msg)
	{
		data = msg;
	}
};

class EngineEvent : public MyEventBase<EngineEvent>
{
public:
	EngineEvent()
	{

	}
	EngineEvent(const std::string &msg)
	{
		data = msg;
	}
};

class InputEvent : public MyEventBase<InputEvent>
{
public:
	InputEvent()
	{
	}

	InputEvent(const std::string &msg)
	{
		data = msg;
	}
};

class DrawEvent : public MyEventBase<DrawEvent>
{
public:
	DrawEvent()
	{
	}

	DrawEvent(const std::string &msg)
	{
		data = msg;
	}
};

//////////////////////////////////////////////////////////////////////////

class SpaceShip
{
public:
	void handle_event(const KeyboardEvent &e)
	{
		std::cout << "KeyboardEvent: " << e.GetData() << std::endl;
	}

	void handle_event(const MouseEvent &e)
	{
		std::cout << "MouseEvent: " << e.GetData() << std::endl;
	}	
};

class RoboCop
{
public:
	void handle_event(const EngineEvent &e)
	{
		std::cout << "EngineEvent: " << e.GetData() << std::endl;
	}

	void handle_event(const MouseEvent &e)
	{
		std::cout << "MouseEvent: " << e.GetData() << std::endl;
	}	
};

class NinjaPirateCyborgJesus
{
public:
	void handle_event(const InputEvent &e)
	{
		std::cout << "InputEvent: " << e.GetData() << std::endl;
	}

	void handle_event(const DrawEvent &e)
	{
		std::cout << "DrawEvent: " << e.GetData() << std::endl;
		std::cout << EventDebugger::inspect(e) << std::endl;
	}	
};

int main()
{		
	EventSystem es;

	SpaceShip ss;
	RoboCop rc;
	NinjaPirateCyborgJesus npcj;

	Connection<NinjaPirateCyborgJesus, DrawEvent> testCon(npcj);

	// reach
	es.RegisterListener(testCon);
	es.EmmitEvent(KeyboardEvent("registred1 (reach)"));
	es.EmmitEvent(DrawEvent("registred2 (reach)"));
	es.ProcessEvents();
	// no reach
	es.RemoveListener(testCon);
	es.EmmitEvent(KeyboardEvent("un registred1 (no reach)"));
	es.ProcessEvents();
	// reach
	es.RegisterListener(testCon);
	es.EmmitEvent(KeyboardEvent("un registred2 (should reach)"));
	// preocess
	es.ProcessEvents();

	return 0;


	es.RegisterListener(Connection<SpaceShip, MouseEvent>(ss));

	es.RegisterListener(Connection<RoboCop, EngineEvent>(rc));
	es.RegisterListener(Connection<RoboCop, MouseEvent>(rc));

	// geht nicht! compiletime error :)
	//es.RegisterListener(Connection<RoboCop, DrawEvent>(rc));

	es.RegisterListener(Connection<NinjaPirateCyborgJesus, InputEvent>(npcj));
	es.RegisterListener(Connection<NinjaPirateCyborgJesus, DrawEvent>(npcj));

	return 0;

	Connection<NinjaPirateCyborgJesus, DrawEvent> callable(npcj);
	callable(DrawEvent("muh"));


}

/*typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::duration<double> sec;
Clock::time_point start = Clock::now();
int counter = 0;
const int runs = 5000;
for (int i = 0; i < runs; i++)
{
	const int eventCount = rand() % 5000;
	for (int k = 0; k < eventCount; k++)
	{
		const int eventType = rand() % 5;
		switch (eventType)
		{
		case 0:
			es.EmmitEvent(MouseEvent("created"));
			break;
		case 1:
			es.EmmitEvent(KeyboardEvent("created"));
			break;
		case 2:
			es.EmmitEvent(EngineEvent("created"));
			break;
		case 3:
			es.EmmitEvent(InputEvent("created"));
			break;
		case 4:
			es.EmmitEvent(DrawEvent("created"));
			break;
		default:
			break;
		}
		counter++;
	}
	es.ProcessEvents();
}

Clock::time_point end = Clock::now();
std::cout << std::fixed << "send : " << counter << " events (" << (int)(counter / sec(end - start).count()) << " e\\s)" << std::endl;*/

/*
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <functional>
//#include <memory>
#include <deque>
#include <sstream>

struct PointerSize
{
	static const int value = sizeof(void*);
};

template<int size = 4>
struct PointerType
{
	typedef unsigned int value_type;
};

template<>
struct PointerType<8>
{
	typedef unsigned long long value_type;
};

*class CallableWrapper
{
public:
	template<typename T>
	CallableWrapper(const T &callable)
		: callable(new WrapperModel<T>(callable))
	{
	}

	void operator()(EventBase *event)
	{
		callable->call(event);
	}
private:
	struct WrapperConcep
	{
		WrapperConcep() {};
		virtual ~WrapperConcep() {};
		virtual void call(EventBase *event) {};
	};

	template<typename T>
	struct WrapperModel : public WrapperConcep
	{
		WrapperModel(const T &callable)
			: callable(callable)
		{}
		virtual ~WrapperModel() {}

		void call(EventBase *event)
		{
			typename T::event_type *e = dynamic_cast<typename T::event_type*>(event);
			if(e)
				callable(e);
		}
	private:
		T callable;
	};

	std::shared_ptr<WrapperConcep> callable;
};

template <typename T>
class Callable
{
public:
	typedef T event_type;

	template<typename T_CALLABLE>
	Callable(T &callable)
		: callable(new CallableModel<T_CALLABLE>(callable))
	{

	}
	void operator()(T *event)
	{
		callable->call(event);
	}
private:
	struct CallableConcept
	{
		CallableConcept() {};
		virtual ~CallableConcept() {};
		virtual void call(T *event) {};
	};

	template<typename T_CALLABLE>
	struct CallableModel : public CallableConcept
	{
		typedef T_CALLABLE object_type;

		CallableModel(T_CALLABLE &callable)
			: callable(callable)
		{}
		virtual ~CallableModel() {}

		void call(T *event)
		{
			callable.handle_event(event);
		}
	private:
		T_CALLABLE callable;
	};

	std::shared_ptr<CallableConcept> callable;
};*

#define EVENT_DEBUGGING

#ifdef EVENT_DEBUGGING
#define EVENT_DEBUG sendLine = __LINE__; \
	sendFunction = __FUNCTION__; \
	sendFile = __FILE__; \
	uuid = "lol-uuid-lol";
#endif

class EventBase
{
public:	
	typedef PointerType<PointerSize::value>::value_type pointer_type;

	virtual pointer_type GetId() const
	{
		return 0;
	}
protected:
	EventBase() {}
	virtual ~EventBase() {};
#ifdef EVENT_DEBUGGING
	std::string GetDebugMsg()
	{
		// "EventBase::GetId:(12) in file main.cpp"
		std::stringstream ss;
		ss << sendLine;
		std::string tmp;
		ss >> tmp;
		return sendFunction + "(" + tmp + ")" + " in file " + sendFile;
	}
	int sendLine;
	std::string sendFunction;
	std::string sendFile;
	std::string uuid;
#endif
};

template<typename T>
class Event : public EventBase
{
public:
	pointer_type GetId() const
	{
		return id;
	}	
protected:    
	Event()
	{
		id = s_id();
	}

	virtual ~Event() 
	{
	}
private:
	static pointer_type s_id()
	{
		return reinterpret_cast<pointer_type>(&s_id);
	}    
	pointer_type id;
};

class KeyboardEvent : public Event<KeyboardEvent>
{
public:
	KeyboardEvent()
		: msg("muh keyboard ist da")
	{
#ifdef EVENT_DEBUGGING
		EVENT_DEBUG
#endif
#ifdef EVENT_DEBUG
			std::cout << GetDebugMsg() << std::endl;
#endif
	}

	std::string GetMessage() const
	{
		return msg;
	}
private:
	std::string msg;
};

class MouseEvent : public Event<MouseEvent>
{
public:
	MouseEvent()
		: msg("hier ist die sendung mit der maus")
	{
#ifdef EVENT_DEBUGGING
		EVENT_DEBUG
#endif
#ifdef EVENT_DEBUG
			std::cout << GetDebugMsg() << std::endl;
#endif
	}

	MouseEvent(const std::string &msg)
		: msg(msg)
	{
	}

	std::string GetMessage() const
	{
		return msg;
	}
private:
	std::string msg;
};

class ListenerBase
{
public:
	//template <typename T> void call(T *event);
	virtual void call(EventBase *event) = 0;
protected:
	virtual ~ListenerBase() {};
};

template<class t_receiver, class t_param>
class Listener : public ListenerBase
{
public:
	typedef t_receiver receiver_type;
	typedef t_param param_type;
	typedef std::function<void (const t_param &e)> listener_type;

	Listener(t_receiver &obj)
		: listener(std::bind(static_cast<void (t_receiver::*)(const t_param& e)>(&t_receiver::handle_event), obj, std::placeholders::_1))
	{

	}

	Listener(const Listener &other)
	{
		if(this != &other)
		{
			this->listener = other.listener;
		}
	}

	~Listener() { };

	Listener &operator=(const Listener &other)
	{
		if(this != &other)
		{
			this->listener = other.listener;
		}

		return *this;
	}

	//void call(param_type *event)
	//{
//
	//}

	void call(EventBase *base)
	{
		param_type *ev = static_cast<param_type*>(base);
		call(ev);
	}

	void call(param_type *mouseEvent)
	{
		listener(*mouseEvent);
	}

	void operator()(const param_type &e)
	{
		listener(e);
	}
private:
	listener_type listener;
};



class EventSystem
{
public:	
	//typedef std::function<void (EventBase*)> listener;

	template<typename T, typename U>
	void RegisterListener(const Listener<T, U> &listener)
	{		
		// create tmp event to get event id, sadly runtype information
		U *event = new U();
		EventBase::pointer_type eventId = event->GetId();

		auto it = eventMapping.find(eventId);
		if(it == eventMapping.end())
		{
			// add
			std::vector<ListenerBase*> v;
			v.push_back(new Listener<T, U>(listener));
			eventMapping.insert(std::make_pair(eventId, v));
		}
		else
		{
			(*it).second.push_back(new Listener<T, U>(listener));
		}
	}

	template<typename T>
	void EmmitEvent(const T &e)
	{
		eventQueue.push_back(new T());
	}

	void ProcessEvents()
	{
		for(auto it = eventQueue.begin(); it != eventQueue.end(); it++)
		{
			EventBase *event = (*it);
			std::vector<ListenerBase*> &listeners = eventMapping[event->GetId()];
			for (auto lIt = listeners.begin(); lIt != listeners.end(); lIt++)
			{
				ListenerBase *lb = *lIt;
				lb->call(event);
			}
		}

		eventQueue.clear();
	}

	template<class R, class P>
	static std::function<void (const P &e)> CreateListener(R &r)
	{
		return std::bind(static_cast<void (R::*)(const P &e)>(&R::handle_event), r, std::placeholders::_1);
	}	
private:
	std::deque<EventBase*> eventQueue;
	std::map<EventBase::pointer_type, std::vector<ListenerBase*>> eventMapping;
};

class Receiver
{
public:
	void handle_event(const KeyboardEvent &e)
	{
		std::cout << "KeyboardEvent: " << e.GetMessage() << std::endl;
	}

	void handle_event(const MouseEvent &e)
	{
		std::cout << "MouseEvent: " << e.GetMessage() << std::endl;
	}	
};

int main()
{		
	EventSystem es;

	Receiver r;

	std::function<void (MouseEvent &e)> listener = std::bind(static_cast<void (Receiver::*)(const MouseEvent& e)>(&Receiver::handle_event), r, std::placeholders::_1);
	std::function<void (MouseEvent &e)> listener2 = EventSystem::CreateListener<Receiver, MouseEvent>(r);

	Listener<Receiver, MouseEvent> l(r);

	es.RegisterListener(Listener<Receiver, KeyboardEvent>(r));
	es.RegisterListener(l);

	es.EmmitEvent(MouseEvent("hihi"));
	es.EmmitEvent(KeyboardEvent());

	es.ProcessEvents();

	KeyboardEvent ke;
	KeyboardEvent ke2;
	MouseEvent me;
	MouseEvent me2("hey");			

	std::cout << "Unique KeyboardEvent id: " << ke.GetId() << std::endl;
	std::cout << "Unique KeyboardEvent id: " << ke2.GetId() << std::endl;
	std::cout << "Unique MouseEvent id: " << me.GetId() << std::endl;
	std::cout << "Unique MouseEvent id: " << me2.GetId() << std::endl;
}
*/

/*#include <iostream>
#include <memory>
#include <vector>
#include <functional>

class Event 
{
public:
	virtual ~Event() {};
};
class EngineEvent : public Event {};
class KeyboardEvent : public Event {};
class MouseEvent : public Event {};

// this gets in: 
// Callable<KeyboardEvent>
class CallableWrapper
{
public:
	template<typename T>
	CallableWrapper(const T &callable)
		: callable(new WrapperModel<T>(callable))
	{
	}

	void operator()(Event *event)
	{
		callable->call(event);
	}
private:
	struct WrapperConcep
	{
		WrapperConcep() {};
		virtual ~WrapperConcep() {};
		virtual void call(Event *event) {};
	};

	template<typename T>
	struct WrapperModel : public WrapperConcep
	{
		WrapperModel(const T &callable)
			: callable(callable)
		{}
		virtual ~WrapperModel() {}

		void call(Event *event)
		{
			typename T::event_type *e = dynamic_cast<T::event_type*>(event);
			if(e)
				callable(e);
		}
	private:
		T callable;
	};

	std::shared_ptr<WrapperConcep> callable;
};

template <typename T>
class Callable
{
public:
	typedef T event_type;

	template<typename T>
	Callable(T &callable)
		: callable(new CallableModel<T>(callable))
	{

	}
	void operator()(T *event)
	{
		callable->call(event);
	}
private:
	struct CallableConcept
	{
		CallableConcept() {};
		virtual ~CallableConcept() {};
		virtual void call(T *event) {};
	};

	template<typename T_CALLABLE>
	struct CallableModel : public CallableConcept
	{
		typedef T_CALLABLE object_type;

		CallableModel(T_CALLABLE &callable)
			: callable(callable)
		{}
		virtual ~CallableModel() {}

		void call(T *event)
		{
			callable.handle_event(event);
		}
	private:
		T_CALLABLE callable;
	};

	std::shared_ptr<CallableConcept> callable;
};

class EventSystem
{
public:
	template<typename C, typename E>
	void RegisterEvent(C &callee)
	{
		// in case of an event:
		// C: type that should 
		// E: type of the event
		// callee: the object to be called
		
		Callable<KeyboardEvent> c1(callee);
		//Callable<EngineEvent> c2(callee);
		c1(new KeyboardEvent());

		CallableWrapper cw((Callable<KeyboardEvent>(callee)));
		cw(new KeyboardEvent());
		cw(new MouseEvent());
		
	}

	void Process()
	{
		// 
	}

	void Emit(Event *e)
	{
		events.push_back(e);
	}
private:
	
	std::vector<CallableWrapper> receiver;
	std::vector<Event*> events;
};

class Receiver
{
public:
	void use(int val)
	{
		std::cout << "(double)" << std::endl;
	}

	void use(Event *e)
	{
		std::cout << "(Event)" << std::endl;
	}

	void handle_event(KeyboardEvent *e)
	{
		std::cout << "(KeyboardEvent)" << std::endl;
	}

	void handle_event(MouseEvent *e)
	{
		std::cout << "(MouseEvent)" << std::endl;
	}
};

int main()
{
	Receiver r;
	EventSystem es;
	es.RegisterEvent<Receiver, KeyboardEvent>(r);
	es.Emit(new KeyboardEvent());
	es.Process();
}*/
