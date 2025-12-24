#include "Pointer.hpp"
#include "../logger/Logger.hpp"

#include <iostream>

Pointer::Pointer(uint64_t pos, uint64_t end) : position(pos), end_point(end) {
}

Pointer::Pointer() : end_point(0) {
}

Pointer::Pointer(uint64_t size) : end_point(size - 1) {
}

Pointer& Pointer::operator=(const Pointer& other) {
    if (this != &other) {
        position = other.position;
        end_point = other.end_point;
    }
    return *this;
}

Pointer::~Pointer() {

}

Pointer::operator uint64_t() const {
	return position;
}

Pointer& Pointer::operator++() {
    ++position;
	if (position > end_point) {
        position = 0;
    }
    return *this;
}

Pointer Pointer::operator++(int) {
	Pointer temp = *this;
	++(*this);
	return temp;
}

Pointer& Pointer::operator--() {
	if (position == 0) {
		position = end_point;
	} else {
		--position;
	}
	return *this;
}

Pointer Pointer::operator--(int) {
	Pointer temp = *this;
	--(*this);
	return temp;
}

Pointer& Pointer::operator+=(const uint64_t arg) {
    position += arg;
	position %= (end_point + 1);

    if (position == 0) {
		if (!Logger::isDebug()) return *this;

		Logger::warn(WarningCode::POINTER_CROSS_BOUNDS, 
					 std::string(". Pointer current position = ") + std::to_string(this->position));
    }
	return *this;
}

Pointer& Pointer::operator-=(const uint64_t arg) {
    if (position < arg) {
		position = (end_point + 1) - (arg - position);

		if (!Logger::isDebug()) return *this;

		Logger::warn(WarningCode::POINTER_CROSS_BOUNDS, 
					 std::string(". Pointer current position = ") + std::to_string(this->position));
    } else {
		position -= arg;
	}
	return *this;
}

Pointer& Pointer::operator=(const uint64_t arg) {
	position = arg % (end_point + 1);

	if (arg > end_point) {

		if (!Logger::isDebug()) return *this;

		Logger::warn(WarningCode::POINTER_CROSS_BOUNDS, 
					 std::string(". Pointer current position = ") + std::to_string(this->position));
	}
    return *this;
}

uint64_t Pointer::size() const {
    return end_point + 1;
}
