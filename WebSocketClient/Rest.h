#pragma once
#include "Utilities.h"
#include <nlohmann/json.hpp>

namespace Rest
{
	using json = nlohmann::json;
	using namespace web::http;
	int getSession(std::string macAddr, std::string streamKey);

	int getRRQ(int session);
};