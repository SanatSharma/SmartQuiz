#include "stdafx.h"
#include "Utilities.h"
#include <cpprest/json.h>


// Creates an HTTP request and returns the response body.
pplx::task<std::string> Utilities::HTTPStreamingAsync(web::uri* url)
{
	http_client client(*url);

	// Make the request and asynchronously process the response.
	return client.request(methods::GET).then([](http_response response)
	{
		if (response.status_code() == status_codes::OK)
		{
			try
			{
				auto body = response.extract_utf8string();
				return body;
			}
			catch (http_exception e)
			{
				std::cout << "Error extracting string. Return \" - 1 \" Error: "<< e.what() << std::endl;
				std::string a = std::string("-1");
				return concurrency::create_task(
					[a]()
				{
					return a;
				});
			}
		}
		else 
		{
			std::cout << "Error connecting to url\n";
			std::string a = std::string("-1");
			return concurrency::create_task(
				[a]()
			{
				return a;
			});
		}
	});
}

std::wstring Utilities::convertToWString(std::string str)
{
	std::wstring tempUrl;
	tempUrl.assign(str.begin(), str.end());
	return tempUrl;
}

bool Utilities::IsJson(std::string str)
{
	json j;
	try
	{
		j = json::parse(str);
		return true;
	}
	catch (json::exception e)
	{
		printf("Input is not json: %s\n", e.what());
		return false;
	}
	return false;	
}
