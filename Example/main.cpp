#include "../EventTest/eventsystem.h"
#include <string>
#include <chrono>
#include <iostream>
#include <iomanip>

//gotcha: the event base class MUST! be a template type, otherwise the static id trick doesn't work and all events get the same id
template<typename T>
class MyEventBase : public es::Event<T>
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

protected:
	std::string data;
};

// simply a shallow copy example
struct uh_uh
{
	uh_uh(const std::string &init)
		: only_one(init)
	{}
	std::string only_one;
};

class KeyboardEvent : public MyEventBase<KeyboardEvent>
{
public:
	KeyboardEvent()
		: key_code('a'), uh(0)
	{
	}

	KeyboardEvent(const std::string &msg)
	{
		data = msg;
	}

	KeyboardEvent(const std::string &msg, const uh_uh *oh)
		: key_code('a'), uh(oh)
	{
		data = msg;
	}

	int key_code;
	const uh_uh *uh;
};

class MouseEvent : public MyEventBase<MouseEvent>
{
public:
	MouseEvent()
		: x(-50), y(50)
	{
	}

	MouseEvent(const std::string &msg)
	{
		data = msg;
	}

	int x;
	int y;
};

class EngineEvent : public MyEventBase<EngineEvent>
{
public:
	EngineEvent()
		: name("super engine"),
		value(+9001)
	{

	}
	EngineEvent(const std::string &msg)
	{
		data = msg;
	}

	std::string name;
	int value;
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

	~InputEvent()
	{

	}

	int mouse_button;
	int down;
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

	bool something_important;
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
	}

	void handle_event()
	{
		std::cout << "NullEvent (no data)" << std::endl;
	}

	void muh_handle(const InputEvent &e)
	{
		std::cout << "InputEvent: " << e.GetData() << std::endl;
	}
};

int main()
{		
	// setup system
	es::EventSystem es;

	// create som listener classes
	SpaceShip ss;
	RoboCop rc;
	NinjaPirateCyborgJesus npcj;

	// create a connection between a class and an event
	es::Connection<NinjaPirateCyborgJesus, DrawEvent> testCon(npcj, &NinjaPirateCyborgJesus::handle_event);

	// some examples, but its pretty intuitive:
	// 1. you create connections and register them
	// 2. you send some events
	// 3. call the process function
	// what you need to keep in mind is: events are not ordered into buckets when saved but are dispatched 
	// (therefore checked) when the processfunction is called, means if you send 10 events, afterwards register a connection
	// for that event type and then process, still all events will reach that event handler!
	// The same hapens with unregister/send/register. imagine the flow: <unregister connection> <send event> <register connection> <process>, the event will
	// not know that our connection was unregistered during its emit and will still fire.
	es.Add(es::Connection<NinjaPirateCyborgJesus, InputEvent>(npcj, &NinjaPirateCyborgJesus::handle_event));
	es.Add(es::Connection<RoboCop, EngineEvent>(rc, &RoboCop::handle_event));
	es.Add(testCon);
	es.Emit(KeyboardEvent("KeyboardEvent (no reach)"));
	es.Emit(DrawEvent("DrawEvent (reach)"));
	es.Process();

	es.Remove(testCon);
	es.Emit(KeyboardEvent("KeyboardEvent (no reach)"));
	es.Emit(DrawEvent("DrawEvent (no reach)"));
	es.Process();

	es.Add(testCon);
	es.Add(es::Connection<SpaceShip, KeyboardEvent>(ss, &SpaceShip::handle_event));
	es.Emit(DrawEvent("DrawEvent (reach)"));
	es.Emit(InputEvent("InputEvent (reach)"));
	es.Emit(KeyboardEvent("KeyboardEvent (reach)", new uh_uh("hello :)")));
	es.Process();

	// shortcut for binding
	auto con = es::bind(npcj, &NinjaPirateCyborgJesus::muh_handle);
	con(InputEvent("hey (bind) :)"));

	// for simply executing functions without events
	es::Connection<NinjaPirateCyborgJesus, es::NullEvent> nullCon(npcj, &NinjaPirateCyborgJesus::handle_event);
	nullCon();
	// or you could do:
	es.Add(nullCon);
	es.Emit(es::NullEvent());
	es.Process();

	// error: no signature matches (our cyborg ninja cant handle keyboard events :/)
	//es::Connection<NinjaPirateCyborgJesus, KeyboardEvent> failCon(npcj, &NinjaPirateCyborgJesus::handle_event);
	//failCon(KeyboardEvent("muh"));
}


