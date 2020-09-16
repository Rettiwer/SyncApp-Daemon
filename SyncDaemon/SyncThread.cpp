#include <libssh2/libssh2.h>
#include <openssl/md5.h>

#include "rsync_client.h"
#include "rsync_entry.h"
#include "rsync_file.h"
#include "rsync_log.h"
#include "rsync_pathutil.h"
#include "rsync_socketutil.h"

#include "SyncThread.h"

#include "RepoController.h"
#include "CommitController.h"

#include "CommitHelper.h"

#include "Utils.h"

#include "Config.h"

#include <chrono>
#include <set>

namespace daemon {

	void SyncThread::sync(Repo& repo) {
		SocketUtil::startup();

		int rc = libssh2_init(0);
		if (rc != 0) {
			LOG_ERROR(LIBSSH2_INIT) << "libssh2 initialization failed: " << rc << LOG_END
		}

		std::string remoteDir = Utils::wstrToUtf8(repo.getRemoteDir());
		if (remoteDir.back() != '/') {
			remoteDir = remoteDir + "/";
		}

		std::string localDir = Utils::wstrToUtf8(repo.getLocalDir());

		int g_cancelFlag = 0;

		SSHIO sshio;

		Config config;
		config.open();

		while (true) {
			if (sshio.isConnected()) {
				auto now = std::chrono::system_clock::now();
				auto last_token = this->server.getUser().getToken().getCreatedAt();
				if (std::chrono::duration_cast<std::chrono::seconds>(now - last_token).count() <= 10) {
					try {
						Client client(&sshio, "rsync", 30, &g_cancelFlag);
						client.setDeletionEnabled(true);

						client.upload(localDir.c_str(), remoteDir.c_str(), 0);

						if (!client.getUpdatedFiles().empty() || !client.getDeletedFiles().empty()) {
							std::shared_ptr<Commit> commit = std::make_shared<Commit>(Commit{});

							std::shared_ptr<Response> response = CommitController::updateCommit(*commit, repo);
							if (response->isSuccesed()) {
								repo.setLocalCommit(*commit);
								config.saveCommit(*commit, repo);
							}
						}
					}
					catch (Exception & e) {
						std::cout << "Sync failed: " << e.getMessage() << std::endl;
					}
				}
			}
			else {
				this->connect(sshio);
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	void SyncThread::performFullSync(Server& server, Repo& repo) {
		SocketUtil::startup();

		int rc = libssh2_init(0);
		if (rc != 0) {
			LOG_ERROR(LIBSSH2_INIT) << "libssh2 initialization failed: " << rc << LOG_END
		}

		std::string remoteDir = Utils::wstrToUtf8(repo.getRemoteDir());
		if (remoteDir.back() != '/') {
			remoteDir = remoteDir + "/";
		}

		std::string localDir = Utils::wstrToUtf8(repo.getLocalDir());

		std::string temporaryFile = PathUtil::join(localDir.c_str(), "acrosync.part");

		int g_cancelFlag = 0;

		std::string ip = this->server.getIp();
		int port = this->server.getPort();
		std::string username = this->server.getUsername().c_str();
		std::string certFile = this->server.getCertificateFileName().c_str();

		SSHIO sshio;
/*
		try {
			std::shared_ptr<Response> response = CommitController::getCommit(repo);
			if (response->isSuccesed()) {
				CommitHelper cH;
				Commit remoteCommit = cH.getCommitFromJson(response->getData());
				//TODO !repo.getLocalCommit
				if (!repo.getLocalCommit || repo.getLocalCommit().getId() != remoteCommit.getId()) {
					sshio.connect(ip.c_str(), port, username.c_str(), 0, certFile.c_str(), 0);
					Client client(&sshio, "rsync", 30, &g_cancelFlag);
					client.setDeletionEnabled(true);
					client.download(localDir.c_str(), remoteDir.c_str(), temporaryFile.c_str(), 0);

					repo.setLocalCommit(remoteCommit);
					Config config;
					config.open();
					config.saveCommit(remoteCommit, repo);
				}
			}
			else {
				server.setLastServerError(response->getMessage());
			}
		}
		catch (Exception& e) {
			std::cout << "Sync failed: " << e.getMessage() << std::endl;
		}*/
	}

	void SyncThread::connect(SSHIO& sshio) {
		std::string ip = this->server.getIp();
		int port = this->server.getPort();
		std::string username = this->server.getUsername().c_str();
		std::string certFile = this->server.getCertificateFileName().c_str();

		try {
			sshio.connect(ip.c_str(), port, username.c_str(), 0, certFile.c_str(), 0);
		}
		catch (Exception & e) {
			std::cout << "Sync failed: " << e.getMessage() << std::endl;
		}
	}

	bool SyncThread::isSyncing(Repo& repo) {
		return this->syncing.find(repo.getId()) != this->syncing.end();
	}

	void SyncThread::addRepoToSync(Repo& repo) {
		if (!isSyncing(repo)) {
			this->syncing.emplace(make_pair(repo.getId(), std::thread(&SyncThread::sync, this, std::ref(repo))));

			this->syncing.find(repo.getId())->second.detach();
		}
	}

	void SyncThread::stopSyncRepos() {
		for (auto it = this->syncing.begin(); it != this->syncing.end(); ++it) {
			it->second.detach();
		}
	}

	bool SyncThread::isSynced(Repo& repo) {
		auto commit = CommitController::getCommit(repo);

		if (commit->isSuccesed()) {
			CommitHelper commitH;
			repo.setRemoteCommit(commitH.getCommitFromJson(commit->getData()));

			if (repo.getLocalCommit().getId() == repo.getRemoteCommit().getId())
				return true;
		}
		return false;
	}
}