#ifndef POINTER_HPP
#define POINTER_HPP
#include <cstdint>

class Pointer {
	uint32_t position = 0;
	uint32_t end_point;

	Pointer(uint32_t pos, uint32_t end);

public:
	Pointer();
	Pointer(uint32_t size);
	Pointer& operator=(const Pointer& other);
   ~Pointer() = default;

	operator uint32_t() const;
	Pointer& operator++();
	Pointer operator++(int);
	Pointer& operator--();
	Pointer operator--(int);
	Pointer& operator+=(const uint32_t arg);
	Pointer& operator-=(const uint32_t arg);
	Pointer& operator=(const uint32_t arg);

	uint32_t size() const;
};

#endif // POINTER_HPP