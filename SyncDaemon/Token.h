#pragma once

#include <chrono>
#include <string>

namespace daemon {
	class Token {
	private:
		std::wstring key;
		std::chrono::system_clock::time_point createdAt;
	public:
		std::wstring getKey() { return this->key; }

		std::chrono::system_clock::time_point getCreatedAt() { return this->createdAt; }

		void setKey(std::wstring key) { this->key = key; }

		void setCreatedAt(std::chrono::system_clock::time_point createdAt) { this->createdAt = createdAt; }
	};
}