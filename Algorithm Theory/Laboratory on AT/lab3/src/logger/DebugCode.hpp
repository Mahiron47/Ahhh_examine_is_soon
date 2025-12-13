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
        POINTER_OUT_OF_BOUNDS = 1
    };
    WarningCode(Value v) : value(v) {}

    const char* what() override {
        switch (value) {
        case POINTER_OUT_OF_BOUNDS:
            return "The sign crossed the border and was moved to the opposite side";
        default:
            return "UNDEFINED";
        }
	}
protected:
    const char* code() override {
        switch (value) {
        case POINTER_OUT_OF_BOUNDS:
            return "POINTER_OUT_OF_BOUNDS";
        default:
            return "UNDEFINED";
        }
    }
private:
    Value value;
};

class ErrorCode : public DebugCode {
public:
    enum Value {
        UNDEFINED_INSTRUCTION = 1
    };
    ErrorCode(Value v) : value(v) {}

    const char* what() override {
        switch (value) {
        case UNDEFINED_INSTRUCTION:
            return "The instruction is not defined";
        default:
            return "UNDEFINED";
        }
    }
protected:
    const char* code() override {
        switch (value) {
        case UNDEFINED_INSTRUCTION:
            return "UNDEFINED_INSTRUCTION";
        default:
            return "UNDEFINED";
        }
	}
private:
    Value value;
};

#endif // !DEBUGCODE_HPP
