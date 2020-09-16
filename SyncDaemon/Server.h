#pragma once

#include "User.h"

#include <string>
#include <vector>

namespace daemon {
	class Server {
	private:
		std::string ip;
		std::string url;
		int port;
		std::string username;
		std::string certificateFileName;

		std::wstring lastServerError;

		User user;
	public:
		std::string getIp() { return this->ip; }

		std::string getUrl() { return this->url; }

		int getPort() { return this->port; }

		std::string getUsername() { return this->username; }

		std::string getCertificateFileName() { return this->certificateFileName; }

		std::wstring& getLastServerError() { return this->lastServerError; }

		User& getUser() { return this->user; }

		void setIp(std::string ip) { this->ip = ip; }

		void setUrl(std::string url) { this->url = url; }

		void setPort(int port) { this->port = port; }

		void setUsername(std::string username) { this->username = username; }

		void setCertificateFileName(std::string certificateFileName) { this->certificateFileName = certificateFileName; }

		void setLastServerError(std::wstring lastServerError) { this->lastServerError = lastServerError; }

		void setUser(User user) { this->user = user; }
	};
}