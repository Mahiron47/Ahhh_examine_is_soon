#ifndef POINTER_HPP
#define POINTER_HPP
#include <cstdint>

class Pointer {
	uint64_t position = 0;
	uint64_t end_point;

public:
	Pointer();
	Pointer(uint64_t size);
   ~Pointer();

	operator uint64_t() const;
	Pointer& operator++();
	Pointer operator++(int);
	Pointer& operator--();
	Pointer operator--(int);
	void operator+(const int arg);
	void operator-(const int arg);
	Pointer& operator=(const int arg);
	operator bool() const;
};

#endif // POINTER_HPP