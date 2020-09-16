#pragma once
#include "vector";

#include "Repo.h";


namespace daemon {
	class DirectoryWatcher {
	private:
		vector<Repo> watched;
		
		void startWatching();
		void isWatching();
	public:
		void addRepoToWatch(Repo &repo);
		void removeRepoToWatch(Repo &repo);
	};
}