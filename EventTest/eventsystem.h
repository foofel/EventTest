#pragma once

#include "event.h"
#include "connection.h"
#include "eventcache.h"

#include <map>
#include <deque>
#include <vector>

class EventSystem
{
public:	
	template<typename T, typename U>
	void RegisterListener(const Connection<T, U> &listener)
	{		
		// create tmp event to get event id, sadly runtype information
		// todo: how to nicer? ^^
		U *event = new U();
		EventBase::id_type eventId = event->GetId();

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
	void RemoveListener(const Connection<T, U> &listener)
	{
		U *event = new U();
		EventBase::id_type eventId = event->GetId();

		auto it = eventMapping.find(eventId);
		if(it != eventMapping.end())
		{
			std::vector<ConnectionBase*> &listeners = (*it).second;
			for (auto lIt = listeners.begin(); lIt != listeners.end();)
			{
				// check, otherwise empty and nothing to do
				if(equals_sig(&listener, static_cast<Connection<T, U>*>(*lIt)))
				{
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
	void EmmitEvent(const T &e)
	{
		T *tmp = static_cast<T*>(eventCache.GetEvent<T>(e.GetId()));
		eventQueue.push_back(new (tmp) T(e));

		//eventQueue.push_back(new T(e));
	}


	void ProcessEvents()
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
			eventCache.HoldEvent(event);

			//delete event;
		}

		eventQueue.clear();
	}

	template<class R, class P>
	static std::function<void (const P &e)> CreateListener(R &r)
	{
		return std::bind(static_cast<void (R::*)(const P &e)>(&R::handle_event), r, std::placeholders::_1);
	}	
private:
	EventCache eventCache;
	std::deque<EventBase*> eventQueue;
	std::map<EventBase::id_type, std::vector<ConnectionBase*>> eventMapping;
};