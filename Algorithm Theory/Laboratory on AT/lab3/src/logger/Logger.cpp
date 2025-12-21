#include "Logger.hpp"
#include "../BrainfuckInterpretator.hpp"

#include <iostream>

bool Logger::is_debug = false;

void Logger::init() {
    is_debug = true;
}

void Logger::log(const char* message) {
    std::cout << "[LOG]: " << message << std::endl;
}

void Logger::warn(WarningCode w) {
    std::cout << "[WARNING]: " << w << " : " << w.what() << " at " << BrainfuckInterpretator::getCodePosition() << " position." << std::endl;
}

void Logger::warn(WarningCode w, const char* message) {
    std::cout << "[WARNING]: " << w << " : " << w.what()
              << message << " at " << BrainfuckInterpretator::getCodePosition() << " position." << std::endl;
}

void Logger::error(ErrorCode e) {
    std::cerr << "[ERROR]: " << e << " : " << e.what() << " at " << BrainfuckInterpretator::getCodePosition() << " position." << std::endl;
}

void Logger::error(ErrorCode e, const char* message) {
    std::cerr << "[ERROR]: " << e << " : " << e.what()
              << message << " at " << BrainfuckInterpretator::getCodePosition() << " position." << std::endl;
}
