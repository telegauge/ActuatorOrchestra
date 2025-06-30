#include "Ukulele.h"
#include <string>
#include <map>

bool is_paused = false; // Global or static variable

Ukulele::Ukulele(const InstrumentConfig &config, std::vector<Adafruit_PWMServoDriver *> &pwm, OledDisplay *oled_display)
		: oled(oled_display)
{
	for (const auto &a : config.pluckers)
	{
		if (a.type == "plucker")
		{
			if (a.bus >= 0 && a.bus < pwm.size())
			{
				Serial.printf("- init plucker: %s (%d) b:%d\n", a.name.c_str(), a.pin, a.bus);
				pluckers.push_back(new Plucker(a.pin, pwm[a.bus], a.name));
			}
		}
	}
	for (const auto &f : config.fretters)
	{
		if (f.type == "fretter")
		{
			if (f.bus >= 0 && f.bus < pwm.size())
			{
				Serial.printf("- init fretter: %s: (%d, %d) b:%d\n", f.name.c_str(), f.pin_left, f.pin_right, f.bus);
				fretters.push_back(new Fretter(f.pin_left, f.pin_right, pwm[f.bus], f.name));
			}
		}
	}
}

Ukulele::~Ukulele()
{
	for (auto *s : pluckers)
		delete s;
	for (auto *f : fretters)
		delete f;
}

void Ukulele::begin()
{
	for (auto *s : pluckers)
		s->begin();
	for (auto *f : fretters)
		f->begin();
}

void Ukulele::loop(int ms)
{
	if (is_paused)
	{
		// Optionally, do nothing or just yield
		return;
	}
	if (command_queue.size() > 0)
	{
		Command cmd = command_queue.front();
		Serial.printf("Command Queue Size: %s %d %d\n", cmd.name.c_str(), cmd.time_ms, ms);
		if (cmd.time_ms <= ms)
		{
			Serial.printf("Command %s\n", cmd.name.c_str());
			if (cmd.name == "strum")
			{
				strum();
			}
			else if (cmd.name == "home")
			{
				home();
			}
			else if (cmd.name == "fret")
			{
				// fret(cmd.args[0], cmd.args[1]);
			}
			else
			{
				Serial.printf("Unknown command %s\n", cmd.name.c_str());
			}
			command_queue.pop();
		}
	}
}

// Send everything to home position
void Ukulele::home()
{
	for (auto *s : pluckers)
		s->home();
	for (auto *f : fretters)
		f->home();
}

// Pluck a specific string
void Ukulele::pluck(int string_num)
{
	if (string_num > 0 && string_num <= numStrings())
		pluckers[string_num - 1]->pluck();
}

// Strum all the strings
void Ukulele::strum()
{
	for (auto *s : pluckers)
	{
		s->pluck();
	}
}

void Ukulele::fret(int fret_num, const String &fingers)
{
	const std::map<std::string, int> fret_map = {
			{"11", 0},
			{"01", 60},
			{"00", 120},
			{"10", 180},
	};

	if (fret_num < 1 || fret_num > numFretters())
		return;

	String key_left = fingers.substring(0, 2);
	String key_right = fingers.substring(2, 4);
	int left_val = fret_map.at(std::string(key_left.c_str()));
	int right_val = fret_map.at(std::string(key_right.c_str()));

	Serial.printf("Fret %d=%s: %s/%s\n", fret_num, fingers.c_str(), key_left.c_str(), key_right.c_str());

	fretters[fret_num - 1]->fret(left_val, right_val);
	oled->grid(0, fret_num - 1, fingers[0] != '0');
	oled->grid(1, fret_num - 1, fingers[1] != '0');
	oled->grid(2, fret_num - 1, fingers[2] != '0');
	oled->grid(3, fret_num - 1, fingers[3] != '0');
}

void Ukulele::chord(const String &pressed)
{

	Serial.printf("Chord %s\n", pressed.c_str());
	for (int fret_num = 1; fret_num <= numFretters(); ++fret_num)
	{
		std::string mask(numStrings(), '0');
		for (int s = 1; s <= numStrings(); ++s)
		{
			int string_fret = pressed[s - 1] - '0';
			// Serial.printf("  >> %s: %d:%d\n", pressed.c_str(), fret_num, string_fret);
			if (string_fret == fret_num)
				mask[s - 1] = '1';
		}
		fret(fret_num, String(mask.c_str()));
	}
}
// }}

// // Fret a chord
// void Ukulele::chord(const String &pressed)
// {
// 	Serial.printf("Chord %s\n", pressed.c_str());

// 	// For each fret from 0 to max_fret, build a mask and call fret(fret_num, mask)
// 	for (int fret_num = 1; fret_num <= numFretters(); ++fret_num)
// 	{
// 		std::string mask(numStrings(), '0');
// 		for (int s = 1; s <= numStrings(); ++s)
// 		{
// 			int string_fret = pressed[s - 1] - '0';
// 			Serial.printf("  >> %s: %d:%d\n", pressed.c_str(), fret_num, string_fret);
// 			if (string_fret == fret_num)
// 				mask[s - 1] = '1';
// 		}
// 		fret(fret_num, String(mask.c_str()));
// 		// Serial.printf("  >>>> %s: %d:%s\n", pressed.c_str(), fret_num, mask.c_str());
// 	}
// }

// // Fret a specific fret
// void Ukulele::fret(int fret_number, const std::vector<bool> &pressed)
// {
// 	Serial.printf("Fret2 %d: %s\n", fret_number, pressed.c_str());

// 	// Use fret_number as needed for your logic
// 	size_t n = fretters.size();
// 	// Serial.printf("::Fret %d %d %d\n", fret_number, pressed.size(), 2 * n);
// 	// if (pressed.size() < 2 * n)
// 	// 	return;

// 	// Serial.print("xx");

// 	std::string key_left;
// 	std::string key_right;

// 	key_left += pressed[0] ? '1' : '0';
// 	key_left += pressed[1] ? '1' : '0';
// 	key_right += pressed[2] ? '1' : '0';
// 	key_right += pressed[3] ? '1' : '0';
// 	const std::map<std::string, int> fret_map = {
// 			{"11", 0},
// 			{"01", 60},
// 			{"00", 120},
// 			{"10", 180},
// 	};

// 	Serial.printf("  Fret %d: %s %s\n", fret_number, key_left.c_str(), key_right.c_str());
// 	fretters[fret_number]->fret(fret_map.at(key_left), fret_map.at(key_right));

// 	oled->grid(0, fret_number - 1, pressed[0]);
// 	oled->grid(1, fret_number - 1, pressed[1]);
// 	oled->grid(2, fret_number - 1, pressed[2]);
// 	oled->grid(3, fret_number - 1, pressed[3]);
// }

// void Ukulele::fret(int fret_number, const String &pressed)
// {
// 	Serial.printf("Fret %d: %s\n", fret_number, pressed.c_str());
// 	std::vector<bool> pressed_vector;
// 	for (size_t i = 0; i < pressed.length(); ++i)
// 		pressed_vector.push_back((pressed[i] - '0') == fret_number);
// 	fret(fret_number, pressed_vector);
// }

int Ukulele::numStrings() const
{
	return pluckers.size();
}

std::string Ukulele::stringName(int idx) const
{
	return (idx >= 0 && idx < (int)pluckers.size()) ? pluckers[idx]->name() : "";
}

int Ukulele::numFretters() const
{
	return fretters.size();
}

std::string Ukulele::fretterName(int idx) const
{
	return (idx >= 0 && idx < (int)fretters.size()) ? fretters[idx]->name() : "";
}