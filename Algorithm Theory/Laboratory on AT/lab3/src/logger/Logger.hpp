#ifndef LOGGER_HPP
#define LOGGER_HPP
#include "DebugCode.hpp"

#include <string>
#include <cstdint>

class Logger {
	static bool is_debug;


public:
	Logger() = default;
   ~Logger() = default;

	static void init();

	static void log(const std::string& message);
	static void warn(WarningCode w);
	static void warn(WarningCode w, const std::string& message);
	static void error(ErrorCode e);
	static void error(ErrorCode e, const std::string& message);
	static bool isDebug() {
		return is_debug;
	}
};

#endif // !LOGGER_HPP
