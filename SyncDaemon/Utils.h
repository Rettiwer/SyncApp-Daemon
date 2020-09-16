#pragma once

#include <string>

#include <sstream>
#include <vector>

namespace daemon {
	class Utils {
	public:
		static std::wstring generateUUID();

		static std::string wstrToUtf8(const std::wstring& utf16);

		static std::wstring strToWstr(const std::string& s);

		static std::vector<std::wstring> split(const std::wstring& s, wchar_t delimiter);
	};
}