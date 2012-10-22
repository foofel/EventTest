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

	void HoldEvent(EventBase *event)
	{
		std::deque<EventBase*> &list = cache[event->GetId()];
		list.push_back(event);
	}

	template<typename T>
	const EventBase *PeekEvent(EventBase::id_type type) const
	{
		std::deque<EventBase*> &list = cache[type];
		if(list.empty())
		{
			list.resize(100);
			for (size_t i = 0; i < list.size(); i++)
			{
				list[i] = new T();
			}
			std::cout << "reallocating 100 elements for id: " << type << std::endl;
		}

		EventBase *elem = list.back();
		return elem;
	}
private:
	std::map<EventBase::id_type, std::deque<EventBase*>> cache;
};