// This file is a part of Akel
// Authors : @kbz_8
// Created : 13/07/2021
// Updated : 07/12/2022

#include <Utils/utils.h>

namespace Ak
{
	__time& Time::getCurrentTime()
	{
		std::time_t t(std::time(0));
		_now = std::localtime(&t);
		_time.sec = static_cast<uint8_t>(_now->tm_sec);
		_time.min = static_cast<uint8_t>(_now->tm_min);
		_time.hour = static_cast<uint8_t>(_now->tm_hour);
		_time.day = static_cast<uint8_t>(_now->tm_mday);
		_time.month = static_cast<uint8_t>(_now->tm_mon + 1);
		_time.year = static_cast<uint32_t>(_now->tm_year + 1900);
		return _time;
	}

	void Time::delay(long pause)
	{
		SDL_Delay(pause);
	}
}
