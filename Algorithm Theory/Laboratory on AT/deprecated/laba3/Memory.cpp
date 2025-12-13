#include "Memory.hpp"
#include <ostream>
#include <iomanip>

Memory::Memory(const char* memory, uint64_t size) {
	this->size = size;
	this->memory = new char[size];
	for (uint64_t i = 0; i < size; i++) {
		this->memory[i] = memory[i];
	}
}

Memory::~Memory() {
	if (memory != nullptr) delete[] memory;
}

char& Memory::operator[](uint64_t index) {
	//TODO: add additinal logic
	return memory[index];
}

Memory& Memory::operator=(const Memory& other) {
	if (this != &other) {
		size = other.size;
		for (uint64_t i = 0; i < size; i++) {
			memory[i] = other.memory[i];
		}
	}
	return *this;
}

void Memory::operator+=(const char other) {
	//TODO : new element with value other initialized
}

void Memory::operator-=(const uint64_t index) {
	//TODO : remove element at index
}

std::ostream& operator<<(std::ostream& os, const Memory& mem) {
	for (uint64_t i = 0; i < mem.size; i++) {
		os << mem.memory[i];
	}
	return os;
}
