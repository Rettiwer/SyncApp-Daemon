#pragma once

#include "Commit.h";

#include <windows.h>
#include <iostream>
#include <memory>

namespace daemon {
	class Repo {
	private:
		std::wstring id;
		std::wstring localDir;
		std::wstring remoteDir;
		Commit localCommit;
		Commit remoteCommit;
		int lockerId;

		std::deque<std::shared_ptr<Commit>> commits;
	public:
		Repo() {}
		Repo(std::wstring repoId) {
			this->id = repoId;
		}

		std::wstring getId() { return this->id; }

		std::wstring getLocalDir() { return localDir; }

		std::wstring getRemoteDir() { return remoteDir; }

		Commit& getLocalCommit() { return this->localCommit; }

		Commit& getRemoteCommit() { return this->remoteCommit; }

		int getLockerId() { return this->lockerId; }

		std::deque<std::shared_ptr<Commit>>& getCommits() { return this->commits; }

		void setId(std::wstring id) { this->id = id; }

		void setLocalDir(std::wstring dir) { this->localDir = dir; }

		void setRemoteDir(std::wstring dir) { this->remoteDir = dir; }

		void setLocalCommit(Commit localCommit) { this->localCommit = localCommit; }

		void setRemoteCommit(Commit remoteCommit) { this->remoteCommit = remoteCommit; }

		void setLockerId(int lockerId) { this->lockerId = lockerId; }
	};
}