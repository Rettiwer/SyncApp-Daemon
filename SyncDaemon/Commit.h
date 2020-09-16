#pragma once

#include <set>
#include <deque>
#include <string>
#include <chrono>

#include "Utils.h"

namespace daemon {
	struct Rename {
		std::string oldName;
		std::string  newName;
		Rename(std::string oldName, std::string newName) {
			this->oldName = oldName;
			this->newName = newName;
		}
	};

	class Commit {
	private:
		std::wstring commitId;

		std::chrono::system_clock::time_point createdAt;
	public:
		Commit() {
			this->commitId = Utils::generateUUID();
			this->createdAt = std::chrono::system_clock::now();
		}

		std::wstring getId() { return this->commitId; }

		std::chrono::system_clock::time_point getCreatedAt() { return this->createdAt; }

		void setId(std::wstring id) { this->commitId = id; }

		void setCreatedAt(std::chrono::system_clock::time_point createdAt) { this->createdAt = createdAt; }
	};
}