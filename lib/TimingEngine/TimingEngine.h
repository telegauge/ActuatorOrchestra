#ifndef TIMINGENGINE_H
#define TIMINGENGINE_H

#include <vector>
#include <functional>
#include <Arduino.h>

struct ScheduledEvent
{
	uint32_t time_ms; // When to trigger (relative to start)
	std::function<void()> action;
};

class TimingEngine
{
public:
	void schedule(uint32_t time_ms, std::function<void()> action);
	void start();
	void update();
	void reset();

private:
	std::vector<ScheduledEvent> events;
	uint32_t startTime;
	bool running = false;
};

#endif // TIMINGENGINE_H