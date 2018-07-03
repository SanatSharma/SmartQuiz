#pragma once
#include "Utilities.h"
#include <nlohmann/json.hpp>
#include "WebSocketClient.h"

namespace Rest
{
	using json = nlohmann::json;
	using namespace web::http;
	int getSession(std::string macAddr, std::string streamKey);

	int getRRQ(int session);

	//("api/{sessionId:int}/{rrqId:int}/saveRRQResponse/{QId:int}/{remoteId}/{response}")
	void postResponse(int remoteId, char* data);

};