#pragma once
#include "stdafx.h"

namespace Rest
{
	int getSession(std::string macAddr, std::string streamKey);

	int getRRQ(int session);
};