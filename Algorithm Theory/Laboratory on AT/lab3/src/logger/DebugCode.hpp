#ifndef DEBUGCODE_HPP
#define DEBUGCODE_HPP

class DebugCode {
public:
    virtual ~DebugCode() = default;
    virtual const char* what() = 0;
    operator const char*() {
        return code();
	}
    
protected:
    virtual const char* code() = 0;

};

class WarningCode : public DebugCode {
public:
    enum Value {
        POINTER_OUT_OF_BOUNDS = 1,
        POINTER_NEGATIVE_SET = 2,
        UNREACHEABLE_POINTER_SIZE = 3
    };
    WarningCode(Value v) : value(v) {}

    const char* what() override;

protected:
    const char* code() override;

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

    const char* what() override;

protected:
    const char* code() override;

private:
    Value value;

};

#endif // !DEBUGCODE_HPP
