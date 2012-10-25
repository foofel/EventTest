#include "event.h"
#include "eventsystem.h"
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

	~InputEvent()
	{

	}

	InputEvent(const InputEvent &other)
	{
		if(this != &other)
		{
			data = other.data;
		}
	}

	InputEvent &operator=(const InputEvent &other)
	{
		if(this != &other)
		{
			data = other.data;
		}

		return *this;
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
		//std::cout << "KeyboardEvent: " << e.GetData() << std::endl;
	}

	void handle_event(const MouseEvent &e)
	{
		//std::cout << "MouseEvent: " << e.GetData() << std::endl;
	}	
};

class RoboCop
{
public:
	void handle_event(const EngineEvent &e)
	{
		//std::cout << "EngineEvent: " << e.GetData() << std::endl;
	}

	void handle_event(const MouseEvent &e)
	{
		//std::cout << "MouseEvent: " << e.GetData() << std::endl;
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
		std::cout << "NullEvent" << std::endl;
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

	// reach
	es.Add(es::Connection<NinjaPirateCyborgJesus, InputEvent>(npcj, &NinjaPirateCyborgJesus::handle_event));
	es.Add(testCon);
	es.Emit(KeyboardEvent("KeyboardEvent (no reach)"));
	es.Emit(DrawEvent("DrawEvent (reach)"));
	es.Process();
	// no reach
	es.Remove(testCon);
	es.Emit(KeyboardEvent("KeyboardEvent (no reach)"));
	es.Emit(DrawEvent("DrawEvent (no reach)"));
	es.Process();
	// reach
	es.Add(testCon);
	es.Emit(DrawEvent("DrawEvent (reach)"));
	es.Emit(InputEvent("InputEvent (reach)"));
	// preocess
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

	// error: no signature matches
	//es::Connection<NinjaPirateCyborgJesus, KeyboardEvent> failCon(npcj, &NinjaPirateCyborgJesus::handle_event);
	//failCon(KeyboardEvent("muh"));
}

/*
int main()
{
	es.Add(Connection<SpaceShip, MouseEvent, &SpaceShip::handle_event>(ss));
	es.Add(Connection<RoboCop, EngineEvent, &RoboCop::handle_event>(rc));
	es.Add(Connection<RoboCop, MouseEvent, &RoboCop::handle_event>(rc));

	// geht nicht! compiletime error :)
	//es.RegisterListener(Connection<RoboCop, DrawEvent>(rc));
	
	typedef std::chrono::high_resolution_clock Clock;
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
				es.Emit(NullEvent());
				break;
			case 1:
				es.Emit(NullEvent());
				break;
			case 2:
				es.Emit(NullEvent());
				break;
			case 3:
				es.Emit(NullEvent());
				break;
			case 4:
				es.Emit(NullEvent());
				break;
			default:
				break;
			}
			counter++;
		}
		es.Process();
	}

	Clock::time_point end = Clock::now();
	double eventsPerSecond = (int)(counter / sec(end - start).count());
	double timePerEventMilli = (1./eventsPerSecond) * 1000 * 1000 * 1000;
	std::cout << std::fixed << std::setprecision(8) << "send : " << counter << " events (" << (int)eventsPerSecond << " e\\s, " << timePerEventMilli << "ns)" << std::endl;

	return 0;
}
}*/