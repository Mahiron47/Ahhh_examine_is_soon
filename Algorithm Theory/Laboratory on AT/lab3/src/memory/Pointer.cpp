#include "Pointer.hpp"
#include "Logger.hpp"
#include <iostream>
#include <cstdio>

Pointer::Pointer(uint64_t size) {
	this->end_point = size - 1;
}

Pointer::~Pointer() {

}

Pointer::operator uint64_t() const {
	return position;
}

Pointer& Pointer::operator++() {
	if (position == end_point) {
		position = 0;
#ifdef LOGGER_HPP
		Logger::warn(WarningCode::POINTER_OUT_OF_BOUNDS,
					 ". Pointer current position = 0", position);
#endif // LOGGER_HPP

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
#ifdef DEBUG
		char* buffer = nullptr;
		Logger::warn(WarningCode::POINTER_OUT_OF_BOUNDS,
					 [this, &buffer]() -> const char* {
				const char* msg = ". Pointer current position = ";
				buffer = new char[64];
				std::snprintf(buffer, 64, "%s%llu", msg, static_cast<unsigned long long>(this->position));
				
				return buffer;
			 }(), position);
		delete[] buffer;
#endif // DEBUG
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

Pointer::operator bool() const {
	return position != end_point;
}
