#pragma once

#include <thread>
#include <atomic>

namespace daemon {
	class SessionThread {
	private:
		std::thread thread;
		std::atomic<bool> running;

		// Sends every 5 seconds controll message to server
		void run();
	public:
		SessionThread() {
			start();
		}

		void start();
		void stop();
	};
}