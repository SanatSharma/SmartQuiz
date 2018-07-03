#include "stdafx.h"
#include "Rest.h"

std::string base_url ="http::/localhost:58081/api/";

int Rest::getSession(std::string macAddr, std::string streamKey)
{

	std::string sessionUrl = base_url + "getSession/" + streamKey + "/" + macAddr;
	
	uri* url = new uri(Utilities::convertToWString(sessionUrl).c_str());
	std::string val = Utilities::HTTPStreamingAsync(url).get();

	std::cout << val << std::endl;

	return std::stoi(val);
}

int Rest::getRRQ(int session)
{
	std::string rrqUrl = base_url + std::to_string(session) + "/getRRQ";

	uri* url = new uri(Utilities::convertToWString(rrqUrl).c_str());
	std::string val = Utilities::HTTPStreamingAsync(url).get();

	std::cout << val << std::endl;


	json j = json::parse(val);
	return int(j["RRQ_ID"]);
}

//("api/{sessionId:int}/{rrqId:int}/saveRRQResponse/{QId:int}/{remoteId}/{response}")
void Rest::postResponse(int remoteId, char * data)
{
	std::string posturl = base_url + std::to_string(SESSION) + "/saveRRQResponse/" + std::to_string(QID) + "/" + std::to_string(remoteId) + "/" + data;

	uri* url = new uri(Utilities::convertToWString(posturl).c_str());
	std::string val = Utilities::HTTPStreamingAsync(url).get();
}
