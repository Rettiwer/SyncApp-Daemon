#pragma once

#include "User.h"
#include "Server.h"

#include <thread>
#include <atomic>

namespace daemon {
	class TokenThread{
	private:
		std::thread thread;
		bool running;

		// Sends every 10 seconds controll message to server
		void run(Server& server);
	public:
		TokenThread(Server& server) {
			start(server);
		}

		void start(Server& server);
		void stop();
	};
}