#include "Pointer.hpp"

#include <iostream>

Pointer::Pointer(uint32_t pos, uint32_t end) : position(pos), end_point(end) {
}

Pointer::Pointer() : end_point(0) {
}

Pointer::Pointer(uint32_t size) : end_point(size - 1) {
}

Pointer& Pointer::operator=(const Pointer& other) {
    if (this != &other) {
        position = other.position;
        end_point = other.end_point;
    }
    return *this;
}

Pointer::operator uint32_t() const {
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

Pointer& Pointer::operator+=(const uint32_t arg) {
    position += arg;
	position %= (end_point + 1);
	return *this;
}

Pointer& Pointer::operator-=(const uint32_t arg) {
    if (position < arg) {
		position = (end_point + 1) - (arg - position);
    } else {
		position -= arg;
	}
	return *this;
}

Pointer& Pointer::operator=(const uint32_t arg) {
	position = arg % (end_point + 1);
    return *this;
}

uint32_t Pointer::size() const {
    return end_point + 1;
}
