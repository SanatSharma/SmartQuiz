#pragma once

#include "Utilities.h"
#include <nlohmann/json.hpp>
#include "WebSocketClient.h"

namespace Rest
{
	using json = nlohmann::json;
	using namespace web::http;
	int getRRQ(int session);

	//("api/{sessionId:int}/{rrqId:int}/saveRRQResponse/{QId:int}/{remoteId}/{response}")
	bool postResponse(int remoteId, const char* data);

};