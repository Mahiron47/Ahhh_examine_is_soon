#ifndef LOGGER_HPP
#define LOGGER_HPP
#include "logger/DebugCode.hpp"

#include <cstdint>

class Logger {
	static bool is_debug;


public:
	Logger() = default;
   ~Logger() = default;

	static void init();

	static void log(const char* message);
	static void warn(WarningCode w);
	static void warn(WarningCode w, const char* message);
	static void error(ErrorCode e);
	static void error(ErrorCode e, const char* message);

	static bool isDebug() {
		return is_debug;
	}
};

#endif // !LOGGER_HPP
