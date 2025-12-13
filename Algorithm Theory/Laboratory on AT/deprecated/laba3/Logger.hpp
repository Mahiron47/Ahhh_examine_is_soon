#ifdef DEBUG

#ifndef LOGGER_HPP
#define LOGGER_HPP
#include "DebugCode.hpp"
#include <cstdint>

class Logger {
public:
	Logger();
   ~Logger();
	static void log(const char* message);
	static void warn(WarningCode w, const char* message, uint64_t pos);
	static void error(ErrorCode e, const char* message, uint64_t pos);
private:
};

#endif // !LOGGER_HPP

#endif // DEBUG
