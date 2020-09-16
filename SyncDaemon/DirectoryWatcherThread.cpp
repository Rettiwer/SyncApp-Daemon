
/*#include "DirectoryWatcherThread.h"
#include "Commit.h"
#include "Utils.h"

#include <chrono>
#include <thread>
#include <mutex>
#include <future>
#include <memory>
#include <windows.h>


#define DIR_WATCH_MASK                                            \
    FILE_NOTIFY_CHANGE_FILE_NAME |  FILE_NOTIFY_CHANGE_LAST_WRITE \
    | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_SIZE 


namespace daemon {
	DWORD DirectoryWatcherThread::processCommit(const char* t_buffer, Repo& t_repo)
	{
		if (NULL == t_buffer) return -1;

		std::shared_ptr<Commit> commit;

		if (!t_repo.getCommits().empty()) {
			auto now = std::chrono::system_clock::now();
			auto last_commit = t_repo.getCommits().back();

			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_commit->getCreatedAt()).count() > 1000) {
				auto newCommit = std::make_shared<Commit>(Commit{});
				t_repo.getCommits().push_back(newCommit);
				commit = newCommit;
			}
			else {
				commit = last_commit;
			}
		}
		else {
			auto newCommit = std::make_shared<Commit>(Commit{});
			t_repo.getCommits().push_back(newCommit);
			commit = newCommit;
		}

		DWORD offset = 0;
		FILE_NOTIFY_INFORMATION* fni = NULL;
		do
		{
			fni = (FILE_NOTIFY_INFORMATION*)(&t_buffer[offset]);
			std::string fileName = Utils::wstrToUtf8(fni->FileName);
			switch (fni->Action)
			{
			case FILE_ACTION_ADDED:
			{
				(*commit).getToAdd().insert(fileName);
			}
			break;
			case FILE_ACTION_REMOVED:
			{
				(*commit).getToRemove().push_back(fileName);
			}
			break;
			case FILE_ACTION_MODIFIED:
			{
				(*commit).getToAdd().insert(fileName);
			}
			break;
			case FILE_ACTION_RENAMED_OLD_NAME:
			{
				(*commit).getToRename().push_back(Rename(fileName, ""));
			}
			break;
			case FILE_ACTION_RENAMED_NEW_NAME:
			{
				(*commit).getToRename().back().newName = fileName;
			}
			break;
			default:
				break;
			}
			offset += fni->NextEntryOffset;
		} while (fni->NextEntryOffset != 0);

		return 0;
	}

	void DirectoryWatcherThread::startWatching(Repo& t_repo) {
		HANDLE hEvents[2];
		HANDLE  hNotify;

		hNotify = FindFirstChangeNotification(t_repo.getLocalDir().c_str(),
			TRUE,
			FILE_NOTIFY_CHANGE_SIZE
		);

		HANDLE hStdIn = ::GetStdHandle(STD_INPUT_HANDLE);
		hEvents[0] = hNotify;
		hEvents[1] = hStdIn;

		HANDLE hDir = CreateFile(
			t_repo.getLocalDir().c_str(),
			FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_DIRECTORY | FILE_FLAG_BACKUP_SEMANTICS,
			NULL
		);


		char acBuf[8192];
		DWORD dwRet;

		for (;;)
		{
			DWORD dwResult = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);

			switch (dwResult)
			{
			case WAIT_OBJECT_0 + 0:
				std::cout << GetLastError() << std::endl;
				break;
			case WAIT_OBJECT_0 + 1:
				if (!t_repo.getCommits().empty()) {
					auto now = std::chrono::system_clock::now();
					auto last_commit = t_repo.getCommits().back();

					if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_commit->getCreatedAt()).count() > 1000) {
						auto newCommit = std::make_shared<Commit>(Commit{});
						t_repo.getCommits().push_back(newCommit);
					}
				}
				else {
					auto newCommit = std::make_shared<Commit>(Commit{});
					t_repo.getCommits().push_back(newCommit);
				}

				memset(acBuf, '\0', sizeof(acBuf));
				ReadDirectoryChangesW(hDir, (LPVOID)acBuf, 8192, TRUE, DIR_WATCH_MASK, &dwRet, NULL, NULL);
				processCommit(acBuf, std::ref(t_repo));
			}
			FindNextChangeNotification(hNotify);
		}
		CloseHandle(hDir);

	}

	bool DirectoryWatcherThread::isWatching(Repo& repo) {
		return watched.find(repo.getId()) != watched.end();
	}

	void DirectoryWatcherThread::addRepoToWatch(Repo& repo) {
		if (!isWatching(repo)) {
			watched.emplace(make_pair(repo.getId(), std::thread(&DirectoryWatcherThread::startWatching, this, std::ref(repo))));

			watched.find(repo.getId())->second.detach();
		}
	}

	void DirectoryWatcherThread::removeRepoToWatch(Repo& repo) {
		if (isWatching(repo))
		{
			watched.find(repo.getId())->second.detach();
		}
	}
}*/