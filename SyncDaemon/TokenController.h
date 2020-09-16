#pragma once

#include "Controller.h"

#include "User.h"

namespace daemon {
	class TokenController {
	public:
		static std::shared_ptr<Response> getToken(User& user);
	};
}