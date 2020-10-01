#include "Config.h"

#include "UserHelper.h"
#include "ServerHelper.h"
#include "RepoHelper.h"
#include "ServerHelper.h"

#include "CommitHelper.h"
#include "CommitController.h"
#include "TokenController.h"

#include "RepoController.h"
#include "RepoHelper.h"

#include "Response.h"

#include "SyncThread.h"

#include <filesystem>
#include <sstream>

namespace daemon {
	void Config::open() {
		this->handler.connect();
	}

	bool Config::createTables() {
		if (!this->handler.checkTable("user") || !this->handler.checkTable("server") 
			|| !this->handler.checkTable("repos") || !this->handler.checkTable("commits")) {
			const char* sql_user = "CREATE TABLE user("  \
				"first_name       TEXT	NOT NULL," \
				"email            TEXT	NOT NULL," \
				"password         TEXT	NOT NULL);";

			const char* sql_server = "CREATE TABLE server("  \
				"ip					TEXT		NOT NULL," \
				"url				TEXT		NOT NULL," \
				"port				INTEGER		NOT NULL," \
				"username			TEXT		NOT NULL," \
				"certificate		TEXT		NOT NULL);";

			const char* sql_repos = "CREATE TABLE repos("  \
				"id				TEXT		NOT NULL," \
				"remote_dir		TEXT		NOT NULL," \
				"local_dir      TEXT		NOT NULL);";

			const char* sql_commits = "CREATE TABLE commits("  \
				"repo_id		TEXT		NOT NULL," \
				"commit_id		TEXT				," \
				"created_at		TEXT				);";

			this->handler.createTable(sql_user);
			this->handler.createTable(sql_server);
			this->handler.createTable(sql_repos);
			this->handler.createTable(sql_commits);

			this->fillConfig();
		}
		return true;
	}

	void Config::saveUser(User& u) {
		std::ostringstream ss;
		ss << "INSERT INTO user VALUES(";
		ss << "'" << Utils::wstrToUtf8(u.getFirstName()) << "'" << ", ";
		ss << "'" << Utils::wstrToUtf8(u.getEmail()) << "'" << ", ";
		ss << "'" << Utils::wstrToUtf8(u.getPassword()) << "'" << ");";

		this->handler.insert(ss.str().c_str());
	}

	User Config::loadUser() {
		const char* sql = "SELECT * FROM user";
		
		UserHelper userH;
		return userH.getUserFromJson(this->handler.select(sql)[0]);
	}

	void Config::saveServer(Server& serverInfo) {
		std::ostringstream ss;
		ss << "INSERT INTO server VALUES(";
		ss << "'" << serverInfo.getIp() << "'" << ", ";
		ss << "'" << serverInfo.getUrl() << "'" << ", ";
		ss << "'" << serverInfo.getPort() << "'" << ", ";
		ss << "'" << serverInfo.getUsername() << "'" << ", ";
		ss << "'" << serverInfo.getCertificateFileName() << "'" << ");";

		this->handler.insert(ss.str().c_str());
	}

	Server Config::loadServer() {
		const char* sql = "SELECT * FROM server";

		ServerHelper serverH;
		return serverH.getServerFromJson(this->handler.select(sql)[0]);
	}

	void Config::saveRepo(Repo& repo) {
		std::ostringstream q1;
		q1 << "INSERT INTO repos VALUES(";
		q1 << "'" << Utils::wstrToUtf8(repo.getId()) << "'" << ", ";
		q1 << "'" << Utils::wstrToUtf8(repo.getRemoteDir()) << "'" << ", ";
		q1 << "'" << Utils::wstrToUtf8(repo.getLocalDir()) << "'" << ");";

		this->handler.insert(q1.str().c_str());
	}

	std::vector<Repo> Config::loadRepos() {
		const char* sql = "SELECT * FROM repos";

		RepoHelper repoH;
		std::vector<Repo> repos;
		auto repo_q = this->handler.select(sql);
		for (auto row : repo_q) {
			Repo repo = repoH.getRepoFromJson(row);

			std::ostringstream ss;
			ss << "SELECT * FROM commits WHERE repo_id = ";
			ss << "'" << Utils::wstrToUtf8(repo.getId()) << "'" << ");";
			auto commit_q = this->handler.select(ss.str().c_str());

			CommitHelper commitH;
			if (!commit_q.empty()) {
				repo.setLocalCommit(commitH.getCommitFromJson(commit_q[0]));
			}

			repos.push_back(repo);
		}
		return repos;
	}

	void Config::saveCommit(Commit& commit, Repo& repo) {
		std::ostringstream q;
		q << "INSERT INTO commits VALUES(";
		q << "'" << Utils::wstrToUtf8(repo.getId()) << "'" << ", ";
		q << "'" << Utils::wstrToUtf8(commit.getId()) << "'" << ", ";
		q << "'" << std::chrono::time_point_cast<seconds>(commit.getCreatedAt()).time_since_epoch().count() << "'" << ");";
		this->handler.insert(q.str().c_str());
	}

	void Config::fillConfig() {
		std::cout << "### Table Server ###" << std::endl;

		Server newServer;

		std::string server_ip;
		std::cout << "Server IP: ";
		std::cin >> server_ip;
		newServer.setIp(server_ip);

		std::string server_url;
		std::cout << "Server URL: ";
		std::cin >> server_url;
		newServer.setUrl(server_url);

		int port;
		std::cout << "Server SSH port: ";
		std::cin >> port;
		newServer.setPort(port);

		std::string server_username;
		std::cout << "Server SSH username: ";
		std::cin >> server_username;
		newServer.setUsername(server_username);

		std::string server_certificate;
		std::cout << "Server SSH certificate file name: ";
		std::cin >> server_certificate;
		newServer.setCertificateFileName(server_certificate);

		this->saveServer(newServer);

		std::cout << "### Table User ###" << std::endl;

		User newUser;

		std::wstring firstName;
		std::cout << "First name: ";
		std::wcin >> firstName;
		newUser.setFirstName(firstName);

		bool loginSuccess = false;

		while (!loginSuccess) {
			std::wstring email;
			std::cout << "Email: ";
			std::wcin >> email;
			newUser.setEmail(email);

			std::wstring password;
			std::cout << "Password: ";
			std::wcin >> password;
			newUser.setPassword(password);

			std::shared_ptr<Response> loginResponse = TokenController::getToken(newUser);
			if (!loginResponse->isSuccesed()) {
				std::wcout << loginResponse->getMessage() << L", reenter" << std::endl;
			}
			else loginSuccess = true;
		}

		this->saveUser(newUser);
		
		bool addNext = true;
		while (addNext) {
			std::cout << "### New repo ###" << std::endl;
			Repo newRepo;

			std::wstring repo_id;
			std::wcout << "Repo ID: ";
			std::wcin >> repo_id;
			newRepo.setId(repo_id);

			std::wstring repo_local_dir;
			std::wcout << "Repo local dir: ";
			std::wcin >> repo_local_dir;
			newRepo.setLocalDir(repo_local_dir);

			std::wstring repo_remote_dir;
			std::wcout << "Repo remote dir: ";
			std::wcin >> repo_remote_dir;
			newRepo.setRemoteDir(repo_remote_dir);

			this->saveRepo(newRepo);

			char nextRepo;
			std::cout << "Do you want add another repo? [Y/N]: ";
			std::cin >> nextRepo;

			if (nextRepo == 'N' || nextRepo == 'n')
				addNext = false;
		}
	}
}
