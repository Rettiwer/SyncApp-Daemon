#include "SqliteHandler.h"
#include "Log.h"

#include <sstream>
#include <vector>
namespace daemon {
	void SqliteHandler::connect() {
		int rc = sqlite3_open("config.db", &db);

		if (rc) {
			std::ostringstream stringStream;
			stringStream << "Can't open database: ";
			stringStream << sqlite3_errmsg(db);

			Log::logError(stringStream.str());
		}
		else {
			Log::logInfo("Opened config database successfully");
		}
	}

	bool SqliteHandler::checkTable(std::string name) {
		char* zErrMsg = 0;

		std::ostringstream sql;
		sql << "SELECT name FROM sqlite_master WHERE type = 'table' AND name = ";
		sql << "'" << name << "'" << ";";
		std::string s = sql.str();
		char* pString = new char[s.length() + 1];
		std::copy(s.c_str(), s.c_str() + s.length() + 1, pString);

		auto data = this->select(pString);
		if (!data.empty()) {
			return true;
		}
		return false;
	}


	bool SqliteHandler::createTable(const char* sql) {
		char* zErrMsg = 0;

		int rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);

		if (rc != SQLITE_OK) {
			std::ostringstream stringStream;
			stringStream << "Can't create table: ";
			stringStream << zErrMsg;

			Log::logError(stringStream.str());
			sqlite3_free(zErrMsg);
			return false;
		}
		return true;
	}

	bool SqliteHandler::insert(const char* sql) {
		char* zErrMsg = 0;

		int rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);

		if (rc != SQLITE_OK) {
			std::ostringstream stringStream;
			stringStream << "Can't insert: ";
			stringStream << zErrMsg;

			Log::logError(stringStream.str());
			sqlite3_free(zErrMsg);
			return false;
		}
		return true;
	}

	std::vector<web::json::value> SqliteHandler::select(const char* sql) {
		std::vector<web::json::value> result;
		sqlite3_stmt* stmt;

		sqlite3_prepare(db, sql, -1, &stmt, NULL);
		sqlite3_step(stmt);
		while (sqlite3_column_text(stmt, 0))
		{
			web::json::value data = web::json::value::object();
			for (int i = 0; i < sqlite3_data_count(stmt); i++) {
				data[utility::conversions::utf8_to_utf16(sqlite3_column_name(stmt, i))] = web::json::value::string(utility::conversions::utf8_to_utf16((char*)sqlite3_column_text(stmt, i)));
			}
			result.push_back(data);
			sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);
		return result;
	}
}