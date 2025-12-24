#include "Memory.hpp"
#include "../logger/Logger.hpp"

Memory::Memory(uint64_t size) {
	this->memory = new char[size];
	this->point = new Pointer(size);
	for (uint64_t i = 0; i < size; i++) {
		this->memory[i] = 0;
	}
}

Memory::Memory(const char* memory, uint64_t size) {
    this->memory = new char[size];
    this->point = new Pointer(size);
    for (uint64_t i = 0; i < size; i++) {
        this->memory[i] = memory[i];
	}
}

Memory::Memory(const Memory& other) {
    memory = new char[other.point->size()];
    for (uint64_t i = 0; i < other.point->size(); i++) {
        memory[i] = other.memory[i];
    }
    point = new Pointer(*other.point);
}

Memory::~Memory() {
	delete point;
	delete[] memory;
}

char& Memory::operator[](uint64_t index) {
	if (index >= point->size()) {
		Logger::error(ErrorCode::INVALID_MEMORY_ACCESS);
		throw ErrorCode(ErrorCode::INVALID_MEMORY_ACCESS);
	}
	return memory[index];
}

Memory& Memory::operator=(const Memory& other) {
	if (this == &other) return *this;

	char* new_mem = new char[other.point->size()];
	for (uint64_t i = 0; i < other.point->size(); i++) new_mem[i] = other.memory[i];

	delete[] memory;
	delete point;
	memory = new_mem;
	point = new Pointer(*other.point);

	return *this;
}

Memory& Memory::operator=(const char other) {
	memory[(uint64_t) *point] = other;
	return *this;
}

Memory& Memory::operator+=(const char other) {
	memory[(uint64_t) *point] += other;
	return *this;
}

Memory& Memory::operator-=(const char other) {
	memory[(uint64_t) *point] -= other;
	return *this;
}

Memory& Memory::operator=(const uint64_t other) {
    *point = other;
    return *this;
}

Memory& Memory::operator+=(const uint64_t other) {
    (*point) += other;
	return *this;
}

Memory& Memory::operator-=(const uint64_t other) {
    (*point) -= other;
	return *this;
}

Memory& Memory::operator++() {
	++(*point);
	return *this;
}

Memory Memory::operator++(int) {
	Memory temp = *this;
	(*point)++;
	return temp;
}

Memory& Memory::operator--() {
	--(*point);
	return *this;
}

Memory Memory::operator--(int) {
	Memory temp = *this;
	(*point)--;
	return temp;
}

Memory::operator char() const {
    return memory[(uint64_t) *point];
}

Memory::operator uint64_t() const {
    return (uint64_t) *point;
}

Memory::operator bool() const {
	return (bool) *point;
}

uint64_t Memory::size() const {
    return point->size();
}

std::ostream& operator<<(std::ostream& os, Memory& mem) {
	uint64_t pos = (uint64_t) mem;
	mem = (uint64_t) 0;
	do { 
		unsigned char out_ch = static_cast<unsigned char>((char) mem);
		if (Logger::isDebug() && (out_ch < 32 || out_ch == 127)) {
			Logger::warn(WarningCode::CONSOLE_SPECIAL_CHARACTERS_DETECTED, 
					 std::string(". Character code = ") + std::to_string(static_cast<uint64_t>(out_ch)) + std::string(" output"));
		}
		os << (char) out_ch;
	} while (mem++, (uint64_t) mem <= pos && (uint64_t) mem != 0);

	return os;
}
