#pragma once

#include "event.h"
#include <map>
#include <deque>

class EventCache
{
public:
	EventCache()
	{
	}

	~EventCache()
	{
		for(auto it = cache.begin(); it != cache.end(); it++)
		{
			std::deque<EventBase*> &list = (*it).second;

			for(auto lIt = list.begin(); lIt != list.end(); lIt++)
			{
				delete *lIt;
			}
		}
	}

	template<typename T>
	// returns a object, ownership transfer
	EventBase *GetEvent(EventBase::id_type type)
	{
		std::deque<EventBase*> &list = cache[type];
		if(list.empty())
		{
			//TODO: whats a good allocation rule?
			list.resize(1);
			for (size_t i = 0; i < list.size(); i++)
			{
				list[i] = new T();
			}
		}

		EventBase *elem = list.back();
		list.pop_back();
		return elem;
	}

	template<typename T>
	// returns a pointer, no ownership transfer!
	const EventBase *PeekEvent(EventBase::id_type type) const
	{
		std::deque<EventBase*> &list = cache[type];
		if(list.empty())
		{
			//TODO: reallocation policy?
			list.resize(25);
			for (size_t i = 0; i < list.size(); i++)
			{
				list[i] = new T();
			}
		}

		EventBase *elem = list.back();
		return elem;
	}

	// moves an object back into the cache
	void HoldEvent(EventBase *event)
	{
		std::deque<EventBase*> &list = cache[event->GetId()];
		list.push_back(event);
	}
private:
	std::map<EventBase::id_type, std::deque<EventBase*>> cache;
};