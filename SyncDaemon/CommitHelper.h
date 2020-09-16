#pragma once

#include <string>
#include <map>
#include <chrono>

namespace daemon {
	class CommitHelper {
	private:
		enum FieldValue { id, createdAt };
		std::map<std::wstring, FieldValue> fieldMap;
		Commit commit;
	public:
		CommitHelper() {
			fieldMap[L"commit_id"] = FieldValue::id;
			fieldMap[L"created_at"] = FieldValue::createdAt;
		}

		void SetField(std::wstring name, web::json::value value)
		{
			switch (fieldMap[name])
			{
			case FieldValue::id: {
				commit.setId(value.as_string());
				break;
			}
			case FieldValue::createdAt: {
				std::chrono::system_clock::time_point createdAt(std::chrono::seconds(std::stoll(value.as_string())));
				commit.setCreatedAt(createdAt);
				break;
			}
			default: break;
			}
		}

		Commit getCommitFromJson(web::json::value json) {
			auto data = json.as_object();
			for (auto it = data.cbegin(); it != data.cend(); ++it)
			{
				const std::wstring& propertyName = it->first;
				const web::json::value& propertyValue = it->second;

				SetField(propertyName, propertyValue);
			}

			return commit;
		}
	};
}