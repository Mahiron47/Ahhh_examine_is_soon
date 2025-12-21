#include "DebugCode.hpp"

const char* WarningCode::what() {
    switch (value) {
    case POINTER_OUT_OF_BOUNDS:
        return "The sign crossed the border and was moved to the opposite side";
    case POINTER_NEGATIVE_SET:
        return "Attempted to set pointer to a negative size. Pointer set to 0";
    case UNREACHEABLE_POINTER_SIZE:
        return "Attempted to set pointer size to an unreachable value";
    default:
        return "UNDEFINED";
    }
}

const char* WarningCode::code() {
    switch (value) {
    case POINTER_OUT_OF_BOUNDS:
        return "POINTER_OUT_OF_BOUNDS";
    case POINTER_NEGATIVE_SET:
        return "POINTER_NEGATIVE_SET";
    case UNREACHEABLE_POINTER_SIZE:
        return "UNREACHEABLE_POINTER_SIZE";
    default:
        return "UNDEFINED";
    }
}

const char* ErrorCode::what() {
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

const char* ErrorCode::code() {
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
