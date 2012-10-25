#include "../EventTest/eventsystem.h"
#include "receiverandevents.h"
#include <chrono>
#include <iostream>
#include <iomanip>

int main()
{
	es::EventSystem es;

	SpaceShip ss;
	RoboCop rc;
	NinjaPirateCyborgJesus npcj;

	es.Add(es::Connection<SpaceShip, MouseEvent>(ss, &SpaceShip::handle_event));
	es.Add(es::Connection<SpaceShip, KeyboardEvent>(ss, &SpaceShip::handle_event));

	es.Add(es::Connection<RoboCop, EngineEvent>(rc, &RoboCop::handle_event));
	es.Add(es::Connection<RoboCop, MouseEvent>(rc, &RoboCop::handle_event));

	es.Add(es::Connection<NinjaPirateCyborgJesus, DrawEvent>(npcj, &NinjaPirateCyborgJesus::handle_event));
	es.Add(es::Connection<NinjaPirateCyborgJesus, InputEvent>(npcj, &NinjaPirateCyborgJesus::handle_event));
	es.Add(es::Connection<NinjaPirateCyborgJesus, es::NullEvent>(npcj, &NinjaPirateCyborgJesus::handle_event));
	es.Add(es::Connection<NinjaPirateCyborgJesus, InputEvent>(npcj, &NinjaPirateCyborgJesus::muh_handle));

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
			const int eventType = rand() % 6;
			switch (eventType)
			{
			case 0:
				es.Emit(KeyboardEvent("iam the keyboard!"));
				break;
			case 1:
				es.Emit(MouseEvent("who who mouse"));
				break;
			case 2:
				es.Emit(EngineEvent("cho cho cho <iam a train>"));
				break;
			case 3:
				es.Emit(InputEvent("left! no right!, aaahhh the other left!"));
				break;
			case 4:
				es.Emit(DrawEvent("show me what u got babe..."));
				break;
			case 5:
				es.Emit(es::NullEvent());
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