#pragma once
#include "Request.h"
#include "Response.h"

#include <string>
#include <vector>

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include <ppltasks.h>

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

namespace daemon {
	class Controller {
	private:
		static std::wstring prepareUrl(std::vector<std::wstring> url);

		static std::shared_ptr<Response> processResponse(web::json::value json);
	public:
		static std::shared_ptr<Response> executeRequest(std::vector<std::wstring> uri, method method, bool authorizationToken = true, json::value body = NULL);
	};
}