#pragma once

#include <sqlite3.h>

#include <cpprest/json.h>

namespace daemon {
	class SqliteHandler {
	private:
		sqlite3* db;
	public:
		void connect();
		bool createTable(const char* sql);
		bool checkTable(std::string name);
		bool insert(const char* sql);

		std::vector<web::json::value> select(const char* sql);
	};
}