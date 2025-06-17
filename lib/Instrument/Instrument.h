#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <queue>
#include <string>
#include <vector>

struct Command
{
	std::string name;
	int time_ms;
	std::vector<int> args;
};

class Instrument
{
public:
	virtual void begin() = 0;
	virtual void loop(int ms) = 0;
	virtual void home() = 0;
	virtual ~Instrument() {}

	virtual void command(std::string name, int time_ms, std::vector<int> args)
	{
		Command cmd;
		cmd.name = name;
		cmd.time_ms = time_ms;
		cmd.args = args;
		Serial.printf("Command Queued %s %d\n", cmd.name.c_str(), cmd.time_ms);
		command_queue.push(cmd);
	}

	std::queue<Command> command_queue;
};

#endif // INSTRUMENT_H