#include "TimingEngine.h"

void TimingEngine::schedule(uint32_t time_ms, std::function<void()> action)
{
	events.push_back({time_ms, action});
}

void TimingEngine::start()
{
	startTime = millis();
	running = true;
}

void TimingEngine::update()
{
	if (!running)
		return;
	uint32_t now = millis() - startTime;
	for (auto it = events.begin(); it != events.end();)
	{
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
	if (events.empty())
	{
		running = false;
	}
}

void TimingEngine::reset()
{
	events.clear();
	running = false;
}