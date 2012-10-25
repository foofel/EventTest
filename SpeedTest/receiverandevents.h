#include <string>
#include "../EventTest/event.h"

/************************************************************************/
/* events                                                               */
/************************************************************************/
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

/************************************************************************/
/* receiver                                                             */
/************************************************************************/
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
	}

	void handle_event(const DrawEvent &e)
	{
	}

	void handle_event()
	{
	}

	void muh_handle(const InputEvent &e)
	{
	}
};