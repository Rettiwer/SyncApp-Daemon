#include "CommunicationServer.h"

namespace daemon {
	void CommunicationServer::handle_get(http_request request) {
		std::vector<std::wstring> path = http::uri::split_path(http::uri::decode(request.relative_uri().path()));

		if (path[0] == L"repo") {
			if (!path[1].empty()) {
				std::wstring uuid = path[1];
				
				Repo repo;
				for (auto it = this->repos.begin(); it < this->repos.end(); it++) {
					if (it->getId() == uuid) {
						repo = *it;
					}
				}

				json::value body;
				body[L"repo_id"] = json::value::string(repo.getId());

				request.reply(status_codes::OK, body);
			}
		}
		request.reply(status_codes::BadRequest);
	}

	void CommunicationServer::handle_post(http_request request) {
		std::wcout << request.extract_json().get() << std::endl;
		request.reply(status_codes::OK);
	}

	void CommunicationServer::handle_put(http_request request) {
		std::wcout << request.to_string() << std::endl;
		request.reply(status_codes::OK);
	}

	void CommunicationServer::handle_delete(http_request request) {
		std::wcout << request.to_string() << std::endl;
		request.reply(status_codes::OK);
	}
}