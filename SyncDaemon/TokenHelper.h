#pragma once

#include "Token.h"

#include <string>
#include <map>

#include <cpprest/json.h>

namespace daemon {

	class TokenHelper {
	private:
		enum FieldValue { key, createdAt };
		std::map<std::wstring, FieldValue> fieldMap;
		Token token;
	public:
		TokenHelper() {
			fieldMap[L"key"] = FieldValue::key;
		}

		void SetField(std::wstring name, web::json::value value)
		{
			switch (fieldMap[name])
			{
			case FieldValue::key:
				token.setKey(value.as_string());
				break;
			}
		}

		Token getTokenFromJson(web::json::value json) {
			auto data = json.as_object();
			for (auto it = data.cbegin(); it != data.cend(); ++it)
			{
				const std::wstring& propertyName = it->first;
				const web::json::value& propertyValue = it->second;

				SetField(propertyName, propertyValue);
			}
			return token;
		}
	};
}