#pragma once

#include <vector>
#include <string>

namespace daemon {
	class ErrorHandler {
	private:
		std::vector<std::wstring> errors;
	public:
		std::vector<std::wstring>& getErrors() { return this->errors; }

		void addError(std::wstring error) { this->errors.push_back(error); }

		void handle(std::wstring error) {
		}
	};
}