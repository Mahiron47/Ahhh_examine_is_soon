#include "Logger.hpp"
#include "../BrainfuckInterpretator.hpp"

#include <windows.h>
#include <iostream>

bool Logger::is_debug = false;

void Logger::init() {
    is_debug = true;
}

void Logger::log(const std::string& message) {
    SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
    
    std::cout << "[LOG] " << message << std::endl;
}

void Logger::warn(WarningCode w) {
    SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

    std::cout << "[WARNING] " << w << " : " << w.what() 
              << " at " << BrainfuckInterpretator::getCodePosition() 
              << " instruction." << std::endl;
}

void Logger::warn(WarningCode w, const std::string& message) {
    SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

    std::cout << "[WARNING] " << w << " : " << w.what()
              << message << " at " << BrainfuckInterpretator::getCodePosition() 
              << " instruction." << std::endl;
}

void Logger::error(ErrorCode e) {
    SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

    std::cout << "[ERROR] " << e << " : " << e.what() 
              << " at " << BrainfuckInterpretator::getCodePosition() 
              << " instruction." << std::endl;

}

void Logger::error(ErrorCode e, const std::string& message) {
    SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

    std::cout << "[ERROR] " << e << " : " << e.what()
              << message << " at " << BrainfuckInterpretator::getCodePosition() 
              << " instruction." << std::endl;
}
