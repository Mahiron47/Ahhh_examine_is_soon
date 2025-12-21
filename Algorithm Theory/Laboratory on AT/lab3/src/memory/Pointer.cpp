#include "Pointer.hpp"
#include "Logger.hpp"

#include <iostream>
#include <cstdio>

Pointer::Pointer() : end_point(0) {
}

Pointer::Pointer(uint64_t size) : end_point(size - 1) {
}

Pointer::~Pointer() {

}

Pointer::operator uint64_t() const {
	return position;
}

Pointer& Pointer::operator++() {
    if (position == end_point) {
        position = 0;

		if (!Logger::isDebug()) return *this;

        Logger::warn(WarningCode::POINTER_OUT_OF_BOUNDS,
                     ". Pointer current position = 0");
    } else {
        ++position;
    }
    return *this;
}

Pointer Pointer::operator++(int) {
	Pointer tmp = *this;
	++(*this);
	return tmp;
}

Pointer& Pointer::operator--() {
	if (position == 0) {
		position = end_point;

		if (!Logger::isDebug()) return *this;

		char* buffer = nullptr;
		Logger::warn(WarningCode::POINTER_OUT_OF_BOUNDS,
					 [this, &buffer]() -> const char* {
				const char* msg = ". Pointer current position = ";
				buffer = new char[64];
				// Copy position to the buffer
				std::snprintf(buffer, 64, "%s%llu", msg, static_cast<unsigned long long>(this->position));
				
				return buffer;
			 }());
		if (buffer != nullptr) delete[] buffer;
	} else {
		--position;
	}
	return *this;
}

Pointer Pointer::operator--(int) {
	Pointer tmp = *this;
	--(*this);
	return tmp;
}

void Pointer::operator+(const int arg) {
    if (end_point + arg > UINT64_MAX) {
        end_point = UINT64_MAX;
        if (!Logger::isDebug()) return;
        Logger::warn(WarningCode::UNREACHEABLE_POINTER_SIZE,
                     ". Pointer size set to UINT64_MAX");
    } else {
        end_point += arg;
    }
}

void Pointer::operator-(const int arg) {
    if (end_point < arg) {
        end_point = 0;
        if (!Logger::isDebug()) return;
        Logger::warn(WarningCode::UNREACHEABLE_POINTER_SIZE,
                     ". Pointer size set to 0");
    } else {
        end_point -= arg;
    }
}
Pointer& Pointer::operator=(const int arg) {
    if (arg < 0) {
		position = 0;
		if (!Logger::isDebug()) return *this;
		Logger::warn(WarningCode::POINTER_NEGATIVE_SET);
	} else {
		position = static_cast<uint64_t>(arg);
	}
    return *this;
}
Pointer::operator bool() const {
    return position != end_point;
}
