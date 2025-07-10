#include "TimingEngine.h"

void TimingEngine::schedule(uint32_t time_ms, std::function<void()> action)
{
	Serial.printf("TimingEngine::schedule %d ms\n", time_ms);
	events.push_back({time_ms, action});
}

void TimingEngine::start()
{
	Serial.printf("TimingEngine::start\n");
	running = true;
}

void TimingEngine::update()
{
	if (!running)
		return;
	uint32_t now = millis(); // Use absolute millis
	// Serial.printf("Timing %d\n", now);
	for (auto it = events.begin(); it != events.end();)
	{
		Serial.printf("Event %d ms:%lu\n", it);
		if (now >= it->time_ms)
		{
			it->action();
			it = events.erase(it);
		}
		else
		{
			++it;
		}
	}
	// if (events.empty())
	// {
	// 	running = false;
	// }
}

void TimingEngine::reset()
{
	Serial.printf("TimingEngine::reset\n");
	events.clear();
	running = false;
}