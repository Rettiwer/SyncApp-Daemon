#pragma once

#include "Controller.h"
#include "Response.h"

#include "Repo.h"
#include "Commit.h"

namespace daemon {
	class CommitController {
	public:
		static std::shared_ptr<Response> getCommit(Repo& repo);
		static std::shared_ptr<Response> updateCommit(Commit& commit, Repo& repo);
	};
}