#include "Logger.hpp"

#include <iostream>

Logger::Logger() {

}

Logger::~Logger() {

}

void Logger::log(const char* message) {
	std::cout << "[LOG]: " << message << std::endl;
}

void Logger::warn(WarningCode w, const char* message, uint64_t pos) {
	std::cout << "[WARNING]: " << w << " : " << w.what() 
			  << message << " at " << pos << " position." << std::endl;
}

void Logger::error(ErrorCode e, const char* message, uint64_t pos) {
	std::cerr << "[ERROR]: " << e << " : " << e.what() 
			  << message << " at " << pos << " position." << std::endl;
}
