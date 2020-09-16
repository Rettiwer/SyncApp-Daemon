#pragma once

#include "Repo.h"

#include "cpprest/json.h"
#include "cpprest/http_listener.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

namespace daemon {
	class CommunicationServer {
	private:
		http_listener listener;

		std::vector<Repo> repos;

		void handle_get(http_request request);
		void handle_post(http_request request);
		void handle_put(http_request request);
		void handle_delete(http_request request);
	public:
		CommunicationServer(std::wstring url, std::vector<Repo> &repos) : listener(url), repos(repos) {
			listener.support(methods::GET, bind(&CommunicationServer::handle_get, this, std::placeholders::_1));
			listener.support(methods::POST, bind(&CommunicationServer::handle_post, this, std::placeholders::_1));
			//listener.support(methods::PUT, bind(&CommunicationServer::handle_put, this, std::placeholders::_1));
			//listener.support(methods::DEL, bind(&CommunicationServer::handle_delete, this, std::placeholders::_1));
		}

		pplx::task<void> open() { return listener.open(); }
		pplx::task<void> close() { return listener.close(); }
	};
}