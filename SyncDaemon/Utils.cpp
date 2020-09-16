#include "Utils.h"

#include <stduuid/uuid.h>

#include <cpprest/json.h>

namespace daemon {

	std::wstring Utils::generateUUID() {
		uuids::uuid const id = uuids::uuid_system_generator{}();
		return uuids::to_string<wchar_t>(id);
	}

	std::string Utils::wstrToUtf8(const std::wstring& utf16) {
		return utility::conversions::utf16_to_utf8(utf16);
	}

	std::wstring Utils::strToWstr(const std::string& s)
	{
		return utility::conversions::utf8_to_utf16(s);
	}

	std::vector<std::wstring> Utils::split(const std::wstring& s, wchar_t delimiter)
	{
		std::vector<std::wstring> tokens;
		std::wstring token;
		std::wstringstream tokenStream(s);
		while (std::getline(tokenStream, token, delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	}
}