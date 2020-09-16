#include "Config.h"

#include "SyncThread.h"
#include "TokenThread.h"

#include "Server.h"
#include "Request.h"
#include "Response.h"
#include "User.h"

#include "CommunicationServer.h"

using namespace daemon;

std::unique_ptr<CommunicationServer> http_ptr;

void setup(std::wstring url, std::vector<Repo> &repos) {
	uri_builder uri(L"http://localhost:2137");
	uri.append_path(url);

	auto addr = uri.to_uri().to_string();
	http_ptr = std::unique_ptr<CommunicationServer>(new CommunicationServer(addr, repos));
	http_ptr->open().wait();
	std::wcout << "Listening at: " << addr << std::endl;
	return;
}

void shutdown() {
	http_ptr->close();

	return;
}

int main() {
	//load config

	//runLocalCommunicationServer

	//getNewToken

	//performFullSync

	//runDirectoryWatchThread

	//runSyncThread

	Config config;
	config.open();
	if (config.createTables()) {
		Server server = config.loadServer();

		User user = config.loadUser();
		server.setUser(user);

		//Runs token thread
		TokenThread tokenThread(server);

		std::vector<Repo> repos = config.loadRepos();

		SyncThread sync(server);

		setup(L"api", repos);

		for (Repo& repo : repos) {
			sync.addRepoToSync(repo);
		}

		while (true) {
			if (!server.getLastServerError().empty()) {
				std::wcout << server.getLastServerError() << std::endl;
			}
			Sleep(1000);
		}
	}
	return 0;
}
