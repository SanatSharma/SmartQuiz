#pragma once

#include "Rest.h"

namespace Quiz
{
	int startUp();
	int quiz_start();
	int quiz_stop();
	int session_end();
	int quiz_poll(int attendance = 0);
}