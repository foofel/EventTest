#pragma once

#include "event.h"

namespace es {

class ConnectionBase
{
public:
	virtual void call(EventBase *event) = 0;
	virtual ~ConnectionBase() {};
};

template<typename T, typename U>
class Connection : public ConnectionBase
{
public:
	typedef T receiver_type;
	typedef U param_type;
	typedef void (receiver_type::*sig_type)(const param_type& e);

	Connection(receiver_type &obj, sig_type signature)
		: calledObject(&obj),
		// cast in case of overloads, this way we automatically get the right overload (of exists)
		// otherwise an compiler error
		//calledFunction(static_cast<void (receiver_type::*)(const param_type& e)>(signature))
		calledFunction(signature)
	{

	}

	Connection(const Connection &other)
	{
		if(this != &other)
		{
			//this->listener = other.listener;
			this->calledObject = other.calledObject;
			this->calledFunction = other.calledFunction;
		}
	}

	virtual ~Connection() { };

	Connection &operator=(const Connection &other)
	{
		if(this != &other)
		{
			//this->listener = other.listener;
			this->calledObject = other.calledObject;
			this->calledFunction = other.calledFunction;
		}

		return *this;
	}

	void call(EventBase *base)
	{
		(calledObject->*calledFunction)(*static_cast<param_type*>(base));
	}

	void operator()(const param_type &e)
	{
		(calledObject->*calledFunction)(e);
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
	receiver_type *calledObject;
	sig_type calledFunction;
};


//TODO: shouldn't this be possible in a better way? its essentially 2x the same stuff
// overload for NULL event
template<typename T>
class Connection<T, NullEvent> : public ConnectionBase
{
public:
	typedef T receiver_type;
	typedef void (receiver_type::*sig_type)();

	Connection(receiver_type &obj, sig_type signature)
		: calledObject(&obj),
		// cast in case of overloads, this way we automatically get the right overload (of exists)
		// otherwise an compiler error
		//calledFunction(static_cast<void (receiver_type::*)(const param_type& e)>(signature))
		calledFunction(signature)
	{

	}

	Connection(const Connection &other)
	{
		if(this != &other)
		{
			//this->listener = other.listener;
			this->calledObject = other.calledObject;
			this->calledFunction = other.calledFunction;
		}
	}

	virtual ~Connection() { };

	Connection &operator=(const Connection &other)
	{
		if(this != &other)
		{
			//this->listener = other.listener;
			this->calledObject = other.calledObject;
			this->calledFunction = other.calledFunction;
		}

		return *this;
	}

	void call(EventBase *base)
	{
		(calledObject->*calledFunction)();
	}

	void operator()()
	{
		(calledObject->*calledFunction)();
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

template<typename T, typename U>
// needs function pointer signature to deduce type for connection object
Connection<T, U> bind(T &obj, void (T::*fctSig)(const U& e))
{
	return Connection<T, U>(obj, fctSig);
}

}; // namespace