#include "CommitController.h"

namespace daemon {
	std::shared_ptr<Response> CommitController::getCommit(Repo& repo) {
		return Controller::executeRequest(std::vector<std::wstring>{L"commit/", repo.getId()}, methods::GET, true);
	}

	std::shared_ptr<Response> CommitController::updateCommit(Commit& commit, Repo& repo) {
		json::value body;
		body[L"repo_id"] = json::value::string(repo.getId());
		body[L"commit_id"] = json::value::string(commit.getId());
		body[L"created_at"] = json::value::number(std::chrono::duration_cast
			<std::chrono::seconds>(commit.getCreatedAt().time_since_epoch()).count());

		return Controller::executeRequest(std::vector<std::wstring>{L"commit"}, methods::PUT, true, body);
	}
}