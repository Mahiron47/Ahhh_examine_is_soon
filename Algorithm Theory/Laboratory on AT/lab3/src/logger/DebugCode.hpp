#ifndef DEBUGCODE_HPP
#define DEBUGCODE_HPP

#include <sstream>
#include <exception>

class DebugCode : public std::exception {
public:
    virtual ~DebugCode() = default;
    const char* what() const noexcept override = 0;
    operator const char*() {
        return code();
	}
    
protected:
    virtual const char* code() const = 0;

};

class WarningCode : public DebugCode {
public:
    enum Value {
        POINTER_CROSS_BOUNDS = 1,
        MEMORY_SIZE_FIXED = 2,
        CONSOLE_SPECIAL_CHARACTERS_DETECTED = 3
    };
    WarningCode(Value v) : value(v) {}

    const char* what() const noexcept override;

protected:
    const char* code() const override;

private:
    Value value;

};

class ErrorCode : public DebugCode {
public:
    enum Value {
        UNDEFINED_INSTRUCTION = 1,
        INPUT_FILE_UNDEFINED = 2,
        INVALID_FILE_PATH = 3,
        INVALID_MEMORY_ACCESS = 4
    };
    ErrorCode(Value v) : value(v) {}

    const char* what() const noexcept override;

protected:
    const char* code() const override;

private:
    Value value;

};

#endif // !DEBUGCODE_HPP
