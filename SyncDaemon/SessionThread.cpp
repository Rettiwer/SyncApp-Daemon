#include "SessionThread.h"

#include <iostream>
#include <chrono>

namespace daemon {
	void SessionThread::run() {
		while (this->running) {
			std::cout << "lol" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	void SessionThread::start() {
		if (!this->running) {
			this->thread = std::thread(&SessionThread::run, this);
			this->thread.detach();
			this->running = true;
		}
	}

	void SessionThread::stop() {
		if (this->running) {
			this->running = false;
		}
	}
}