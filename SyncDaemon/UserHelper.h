#pragma once

#include "User.h"

#include <string>
#include <map>

#include <cpprest/json.h>

namespace daemon {
	class UserHelper {
	private:
		enum FieldValue { id, firstName, email, password };
		std::map<std::wstring, FieldValue> fieldMap;
		User user;
	public:
		UserHelper() {
			fieldMap[L"id"] = FieldValue::id;
			fieldMap[L"first_name"] = FieldValue::firstName;
			fieldMap[L"email"] = FieldValue::email;
			fieldMap[L"password"] = FieldValue::password;
		}

		void SetField(std::wstring name, web::json::value value)
		{
			switch (fieldMap[name])
			{
			case FieldValue::id:
				user.setId(value.as_integer());
				break;

			case FieldValue::firstName:
				user.setFirstName(value.as_string());
				break;

			case FieldValue::email:
				user.setEmail(value.as_string());
				break;
			case FieldValue::password:
				user.setPassword(value.as_string());
				break;
			}
		}

		User getUserFromJson(web::json::value json) {
			auto data = json.as_object();
			for (auto it = data.cbegin(); it != data.cend(); ++it)
			{
				const std::wstring& propertyName = it->first;
				const web::json::value& propertyValue = it->second;

				SetField(propertyName, propertyValue);
			}
			return user;
		}
	};
}