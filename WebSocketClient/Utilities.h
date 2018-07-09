#pragma once

#include <cpprest/http_client.h>
#include <nlohmann/json.hpp>
#include <ppltasks.h>

// Utility namespace where general utility functions can be put
namespace Utilities
{
	using namespace web::http;
	using namespace web::http::client;
	using json = nlohmann::json;

	Concurrency::task<std::string> HTTPStreamingAsync(uri* url);

	std::wstring convertToWString(std::string str);

	bool IsJson(std::string str);
}
