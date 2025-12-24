#ifndef POINTER_HPP
#define POINTER_HPP
#include <cstdint>

class Pointer {
	uint64_t position = 0;
	uint64_t end_point;

	Pointer(uint64_t pos, uint64_t end);

public:
	Pointer();
	Pointer(uint64_t size);
	Pointer& operator=(const Pointer& other);
   ~Pointer();

	operator uint64_t() const;
	Pointer& operator++();
	Pointer operator++(int);
	Pointer& operator--();
	Pointer operator--(int);
	Pointer& operator+=(const uint64_t arg);
	Pointer& operator-=(const uint64_t arg);
	Pointer& operator=(const uint64_t arg);

	uint64_t size() const;
};

#endif // POINTER_HPP