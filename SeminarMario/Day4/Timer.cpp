#include "Timer.h"
#include "opencv2/opencv.hpp"
#include "Config.h"

using namespace cv;

Timer::Timer(size_t frequency_ms)
	:_frequency_ms(frequency_ms)
{
}

void Timer::tick()
{
	int key = waitKey(_frequency_ms);

	Notify(Event{ EventSenders::SENDER_TIMER, EventTypes::EVENT_TIMER , EventCodes::TIME_TICK });
	if (key != -1)
		Notify(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED , key });

	if (key == KEY_LEFT || key == KEY_RIGHT || key == KEY_DOWN || key == KEY_UP) {
		Notify(Event{ EventSenders::SENDER_TIMER, EventTypes::EVENT_KEY_PRESSED, EventCodes::COLLISION_WITH_FOOD_SCORE });
	}
	

	if (key == KEY_ESC)
		exit(0);
}
