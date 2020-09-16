#include "RepoController.h"

#include <sstream>

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
	std::shared_ptr<Response> RepoController::getRepo(Repo& repo) {
		return Controller::executeRequest(std::vector<std::wstring>{L"repo/", repo.getId()}, methods::GET);
	}
}
