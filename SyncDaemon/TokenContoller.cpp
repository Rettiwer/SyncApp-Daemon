#include "TokenController.h"

namespace daemon {
	std::shared_ptr<Response> TokenController::getToken(User& user) {
		json::value body;
		body[L"email"] = json::value::string(user.getEmail());
		body[L"password"] = json::value::string(user.getPassword());

		return Controller::executeRequest(std::vector<std::wstring>{L"token"}, methods::POST, false, body);
	}
}