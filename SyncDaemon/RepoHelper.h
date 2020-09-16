#pragma once

#include "Repo.h"

#include <string>
#include <map>

#include <cpprest/json.h>

namespace daemon {
	class RepoHelper {
	private:
		enum FieldValue { id, localDir, remoteDir, lockerId };
		std::map<std::wstring, FieldValue> fieldMap;
		Repo repo;
	public:
		RepoHelper() {
			fieldMap[L"id"] = FieldValue::id;
			fieldMap[L"local_dir"] = FieldValue::localDir;
			fieldMap[L"remote_dir"] = FieldValue::remoteDir;
			fieldMap[L"locker"] = FieldValue::lockerId;
		}

		void SetField(std::wstring name, web::json::value value)
		{
			switch (fieldMap[name])
			{
			case FieldValue::id: 
				repo.setId(value.as_string());
				break;
			case FieldValue::localDir:
				repo.setLocalDir(value.as_string().c_str());
				break;
			case FieldValue::remoteDir:
				repo.setRemoteDir(value.as_string().c_str());
				break;
			case FieldValue::lockerId:
				repo.setLockerId(std::stoi(value.as_string()));
				break;

			default:break;
			}
		}

		Repo getRepoFromJson(web::json::value json) {
			auto data = json.as_object();
			for (auto it = data.cbegin(); it != data.cend(); ++it)
			{
				const std::wstring propertyName = it->first;
				const web::json::value propertyValue = it->second;
				
				SetField(propertyName, propertyValue);
			}

			return repo;
		}
	};
}