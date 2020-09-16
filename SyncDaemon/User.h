#pragma once

#include <string>
#include "Token.h"

namespace daemon {
	class User {
	private:
		int id;
		std::wstring firstName;
		std::wstring email;
		std::wstring password;

		Token token;
	public:
		int getId() { return this->id; }

		std::wstring getFirstName() { return this->firstName; }

		std::wstring getEmail() { return this->email; }

		std::wstring getPassword() { return this->password; }

		Token& getToken() { return this->token; }
	
		void setId(int id) { this->id = id; }
		void setFirstName(std::wstring firstName) { this->firstName = firstName; }
		void setEmail(std::wstring email) { this->email = email; }
		void setPassword(std::wstring password) { this->password = password; }
		void setToken(Token token) { this->token = token; }
	};
}