#include "TokenThread.h"

#include "TokenHelper.h"
#include "TokenController.h"

#include <iostream>
#include <chrono>
#include <algorithm>

namespace daemon {
	void TokenThread::run(Server& server) {
		while (this->running) {
			User& user = server.getUser();

			std::shared_ptr<Response> response = TokenController::getToken(user);
			if (response->isSuccesed()) {
				TokenHelper tH;
				Token newToken = tH.getTokenFromJson(response->getData());
				newToken.setCreatedAt(std::chrono::system_clock::now());

				user.setToken(newToken);

				
				if (!server.getLastServerError().empty()) {
					server.getLastServerError().clear();
					std::cout << "Cleared" << std::endl << std::endl << std::endl;
				}

				std::this_thread::sleep_for(std::chrono::seconds(10));
			}
			else {
				server.setLastServerError(response->getMessage());
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
	}

	void TokenThread::start(Server& server) {
		if (!this->running) {
			this->running = true;
			this->thread = std::thread(&TokenThread::run, this, std::ref(server));
			this->thread.detach();
		}
	}

	void TokenThread::stop() {
		if (this->running) {
			this->running = false;
		}
	}
}