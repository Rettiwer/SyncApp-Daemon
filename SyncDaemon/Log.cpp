#include "Log.h"

#include <fstream>
#include <time.h>

namespace daemon {
	void Log::logError(std::string message) {
		Log::log(" [Error] ", message);
	}

	void Log::logInfo(std::string message) {
		Log::log(" [Info] ", message);
	}

	void Log::log(std::string prefix, std::string message) {
		std::fstream logFile("log.txt", std::fstream::in | std::fstream::out | std::fstream::app);

		if (logFile.good()) {
			char str[100];
			time_t rawtime;
			struct tm timeinfo;
			time(&rawtime);
			localtime_s(&timeinfo, &rawtime);
			strftime(str, 100, "%Y-%m-%d %X", &timeinfo);

			logFile << str << prefix << message << std::endl;
			logFile.close();
		}
	}
}