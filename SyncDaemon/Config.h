#pragma once

#include "Utils.h"
#include "SqliteHandler.h"
#include "User.h"
#include "Server.h"
#include "Repo.h"

#include <sqlite3.h>

namespace daemon {
	class Config {
	private:
		SqliteHandler handler;
	public:
		void open();

		bool createTables();

		void saveUser(User& u);
		User loadUser();

		void saveServer(Server& serverInfo);
		Server loadServer();

		void saveRepo(Repo& repo);
		std::vector<Repo> loadRepos();

		void saveCommit(Commit& commit, Repo& repo);

		void fillConfig();
	};
}