#pragma once

#include "rsync_sshio.h"

#include "Repo.h"
#include "Server.h"

#include <string>
#include <map>
#include <thread>

using namespace rsync;

namespace daemon {
	class SyncThread {
	private:
		Server& server;
		std::map<std::wstring, std::thread> syncing;

		void connect(SSHIO& sshio);

		void sync(Repo& repo);
		bool isSyncing(Repo& repo);
	public:
		SyncThread(Server& server) : server(server) {}

		void performFullSync(Server& server, Repo& repo);

		void addRepoToSync(Repo& repo);
		void stopSyncRepos();

		static bool isSynced(Repo& repo);
	};
}