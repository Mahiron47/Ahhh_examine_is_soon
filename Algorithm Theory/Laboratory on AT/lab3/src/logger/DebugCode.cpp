#include "DebugCode.hpp"

const char* WarningCode::what() const noexcept {
    switch (value) {
    case POINTER_CROSS_BOUNDS:
        return "The sign crossed the border and was moved to the opposite side";
    case MEMORY_SIZE_FIXED:
        return "Memory size has been fixed after memory pointer cross bounds";
    case CONSOLE_SPECIAL_CHARACTERS_DETECTED:
        return "Special characters detected in console input whose may change console stream behavior";
    default:
        return "UNDEFINED";
    }
}

const char* WarningCode::code() const {
    switch (value) {
    case POINTER_CROSS_BOUNDS:
        return "POINTER_CROSS_BOUNDS";
    case MEMORY_SIZE_FIXED:
        return "MEMORY_SIZE_FIXED";
    case CONSOLE_SPECIAL_CHARACTERS_DETECTED:
        return "CONSOLE_SPECIAL_CHARACTERS_DETECTED";
    default:
        return "UNDEFINED";
    }
}

const char* ErrorCode::what() const noexcept {
    switch (value) {
    case UNDEFINED_INSTRUCTION:
        return "The instruction is not defined";
    case INPUT_FILE_UNDEFINED:
        return "Flag \"-i\" is not set. Please set it to use input file. Occured";
    case INVALID_FILE_PATH:
        return "Flag \"-i\" is set but the file path is invalid. Occured";
    case INVALID_MEMORY_ACCESS:
        return "Attempted to access memory which is undefined";
    default:
        return "UNDEFINED";
    }
}

const char* ErrorCode::code() const {
    switch (value) {
    case UNDEFINED_INSTRUCTION:
        return "UNDEFINED_INSTRUCTION";
    case INPUT_FILE_UNDEFINED:
        return "INPUT_FILE_UNDEFINED";
    case INVALID_FILE_PATH:
        return "INVALID_FILE_PATH";
    case INVALID_MEMORY_ACCESS:
        return "INVALID_MEMORY_ACCESS";
    default:
        return "UNDEFINED";
    }
}
