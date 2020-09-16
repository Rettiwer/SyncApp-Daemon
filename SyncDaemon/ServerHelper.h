#pragma once

#include "Server.h"

#include "Utils.h"

#include <string>
#include <map>

#include <cpprest/json.h>

namespace daemon {

	class ServerHelper {
	private:
		enum FieldValue { ip, url, port, username, certificate };
		std::map<std::wstring, FieldValue> fieldMap;
		Server server;
	public:
		ServerHelper() {
			fieldMap[L"ip"] = FieldValue::ip;
			fieldMap[L"url"] = FieldValue::url;
			fieldMap[L"port"] = FieldValue::port;
			fieldMap[L"username"] = FieldValue::username;
			fieldMap[L"certificate"] = FieldValue::certificate;
		}

		void SetField(std::wstring name, web::json::value value)
		{
			switch (fieldMap[name])
			{
			case FieldValue::ip:
				server.setIp(utility::conversions::to_utf8string(value.as_string()));
				break;
			case FieldValue::url:
				server.setUrl(utility::conversions::to_utf8string(value.as_string()));
				break;
			case FieldValue::port:
				server.setPort(std::stoi(value.as_string()));
				break;
			case FieldValue::username:
				server.setUsername(utility::conversions::to_utf8string(value.as_string()));
				break;
			case FieldValue::certificate:
				server.setCertificateFileName(utility::conversions::to_utf8string(value.as_string()));
				break;
			}
		}

		Server getServerFromJson(web::json::value json) {
			auto data = json.as_object();
			for (auto it = data.cbegin(); it != data.cend(); ++it)
			{
				const std::wstring& propertyName = it->first;
				const web::json::value& propertyValue = it->second;

				SetField(propertyName, propertyValue);
			}
			return server;
		}
	};
}