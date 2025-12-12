#ifndef POINTER_HPP
#define POINTER_HPP
#include <cstdint>

#ifdef DEBUG
class Pointer {
public:
	Pointer(uint64_t size);
   ~Pointer();

	operator uint64_t() const;
	Pointer& operator++();
	Pointer operator++(int);
	Pointer& operator--();
	Pointer operator--(int);
	operator bool() const;
private:
	uint64_t position = 0;
	uint64_t end_point;
	uint64_t code_point;
};
#endif // DEBUG

#ifndef DEBUG

class Pointer {
public:
	Pointer(uint64_t size);
   ~Pointer();

	operator uint64_t() const;
	Pointer& operator++();
	Pointer operator++(int);
	Pointer& operator--();
	Pointer operator--(int);
	operator bool() const;
private:
	uint64_t position = 0;
	uint64_t end_point;
};

#endif // DEBUG

#endif // POINTER_HPP