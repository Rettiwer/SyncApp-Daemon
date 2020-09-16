/*#pragma once
#include <map>

#include <thread>
#include "Repo.h"

namespace daemon {
	class DirectoryWatcherThread {
	private:
		std::map<std::wstring, std::thread> watched;
		
		DWORD processCommit(const char* t_buffer, Repo& t_repo);
		void startWatching(Repo& t_repo);

		bool isWatching(Repo& t_repo);
	public:
		void addRepoToWatch(Repo& t_repo);
		void removeRepoToWatch(Repo& t_repo);
	};
}*/