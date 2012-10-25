#pragma once

#include "event.h"
#include "connection.h"
#include "eventcache.h"

#include <map>
#include <deque>
#include <vector>

namespace es {

template<typename T>
class IDLookup
{
public:
	static EventBase::id_type GetId()
	{
		// create object to get event id, sadly runtype information
		static T event;
		EventBase::id_type eventId = event.GetId();
		return eventId;
	}
};

class EventSystem
{
public:	
	template<typename T, typename U>
	void Add(const Connection<T, U> &listener)
	{		
		EventBase::id_type eventId = IDLookup<U>::GetId();

		auto it = eventMapping.find(eventId);
		if(it != eventMapping.end())
		{
			// add
			(*it).second.push_back(new Connection<T, U>(listener));
		}
		else
		{
			// new
			std::vector<ConnectionBase*> v;
			v.push_back(new Connection<T, U>(listener));
			eventMapping.insert(std::make_pair(eventId, v));
		}
	}

	template<typename T, typename U>
	void Remove(const Connection<T, U> &listener)
	{
		EventBase::id_type eventId = IDLookup<U>::GetId();

		auto it = eventMapping.find(eventId);
		if(it != eventMapping.end())
		{
			std::vector<ConnectionBase*> &listeners = (*it).second;
			for (auto lIt = listeners.begin(); lIt != listeners.end();)
			{
				// check, otherwise empty and nothing to do
				if(equals_sig(&listener, static_cast<Connection<T, U>*>(*lIt)))
				{
					ConnectionBase *con = *lIt; //static_cast<Connection<T, U>*>(*lIt);
					delete con;
					lIt = listeners.erase(lIt);
				}
				else
				{
					lIt++;
				}
			}
		}
	}

	template<typename T>
	void Emit(const T &e)
	{
		// get element from cache
		T *tmp = static_cast<T*>(eventCache.FetchEvent<T>(e.GetId()));
		// placement new
		eventQueue.push_back(new (tmp) T(e));
	}

	void Process()
	{
		for(auto it = eventQueue.begin(); it != eventQueue.end(); it++)
		{
			EventBase *event = (*it);
			std::vector<ConnectionBase*> &listeners = eventMapping[event->GetId()];
			for (auto lIt = listeners.begin(); lIt != listeners.end(); lIt++)
			{
				ConnectionBase *lb = *lIt;
				lb->call(event);
			}
			// move back into the cache
			eventCache.ReturnEvent(event);
		}

		eventQueue.clear();
	}
private:
	EventCache eventCache;
	std::deque<EventBase*> eventQueue;
	std::map<EventBase::id_type, std::vector<ConnectionBase*>> eventMapping;
};

}; // namespace