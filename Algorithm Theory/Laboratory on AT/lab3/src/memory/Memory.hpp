#ifndef MEMORY_HPP
#define MEMORY_HPP
#include "Pointer.hpp"

#include <cstdint>
#include <ostream>

class Memory {
	bool is_debug;
	char* memory;
	Pointer* point;

public:
	Memory(uint64_t size);
	Memory(const char* memory, uint64_t size);
	Memory(const Memory& other);
   ~Memory();

	char& operator[](uint64_t index);
	Memory& operator=(const Memory& other);
	Memory& operator=(const char other);
	Memory& operator+=(const char other);
	Memory& operator-=(const char other);
	Memory& operator=(const uint64_t other);
	Memory& operator+=(const uint64_t other);
	Memory& operator-=(const uint64_t other);
	Memory& operator++();
	Memory operator++(int);
	Memory& operator--();
	Memory operator--(int);

	operator char() const;
	operator uint64_t() const;
	operator bool() const;
	uint64_t size() const;

	friend std::ostream& operator<<(std::ostream& os, Memory& mem);
};

#endif // !MEMORY_HPP
