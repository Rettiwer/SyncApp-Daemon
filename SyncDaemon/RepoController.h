#pragma once

#include "Controller.h"

#include "Repo.h"

namespace daemon {
	class RepoController {
	public:
		static std::shared_ptr<Response> getRepo(Repo& repo);

		static std::shared_ptr<Response> getRepoLocker(Repo& repo);

		static bool renameFile(Repo& repo, Rename& rename);
	};
}