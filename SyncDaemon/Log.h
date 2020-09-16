#pragma once

#include <string>

namespace daemon {
	class Log {
	private:
		static void log(std::string prefix, std::string message);
	public:
		static void logError(std::string message);
		static void logInfo(std::string message);
	};
}