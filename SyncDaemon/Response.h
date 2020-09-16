#pragma once

#include <cpprest/json.h>

namespace daemon {
	class Response {
	private:
		bool success;
		std::wstring message;
		web::json::value data;
	public:
		Response(bool success, std::wstring message, web::json::value data) : success(success),
			message(message), data(data) {}

		bool isSuccesed() { return this->success; }
		std::wstring getMessage() { return this->message; }
		web::json::value getData() { return this->data; }

		void setIsSuccesed(bool success) { this->success = success; }
		void setMessage(std::wstring message) { this->message = message; }
 	};
}