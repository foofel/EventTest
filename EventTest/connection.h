#pragma once

#include "event.h"
#include <functional>

class ConnectionBase
{
public:
	//template <typename T> void call(T *event);
	virtual void call(EventBase *event) = 0;
protected:
	virtual ~ConnectionBase() {};
};

template<class receiver_type, class param_type>
class Connection : public ConnectionBase
{
public:

	typedef std::function<void (const param_type &e)> listener_type;
	typedef void (receiver_type::*sig_type)(const param_type& e);

	Connection(receiver_type &obj)
		: listener(std::bind(static_cast<void (receiver_type::*)(const param_type& e)>(&receiver_type::handle_event), obj, std::placeholders::_1)),
		calledObject(&obj),
		calledFunction(static_cast<void (receiver_type::*)(const param_type& e)>(&receiver_type::handle_event))
	{

	}

	Connection(const Connection &other)
	{
		if(this != &other)
		{
			this->listener = other.listener;
			this->calledObject = other.calledObject;
			this->calledFunction = other.calledFunction;
		}
	}

	~Connection() { };

	Connection &operator=(const Connection &other)
	{
		if(this != &other)
		{
			this->listener = other.listener;
			this->calledObject = other.calledObject;
			this->calledFunction = other.calledFunction;
		}

		return *this;
	}

	void call(EventBase *base)
	{
		call(static_cast<param_type*>(base));
	}

	void call(param_type *mouseEvent)
	{
		listener(*mouseEvent);
	}

	void operator()(const param_type &e)
	{
		listener(e);
	}

	receiver_type *GetReceiver() const
	{
		return calledObject;
	}

	sig_type GetFunctionPointer() const
	{
		return calledFunction;
	}
private:
	listener_type listener;
	receiver_type *calledObject;
	sig_type calledFunction;
};

// compare using only object, not the function signatures themselves
template<typename T, typename U>
bool equals_object(const Connection<T, U> *lhs, const Connection<T, U> *rhs)
{
	return lhs->GetReceiver() == rhs->GetReceiver();
}

// compare using object and function signature
template<typename T, typename U>
bool equals_sig(const Connection<T, U> *lhs, const Connection<T, U> *rhs)
{
	return lhs->GetReceiver() == rhs->GetReceiver() && lhs->GetFunctionPointer() == rhs->GetFunctionPointer();
}