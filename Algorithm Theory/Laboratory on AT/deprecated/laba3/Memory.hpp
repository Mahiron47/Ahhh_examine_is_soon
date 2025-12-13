#ifndef MEMORY_HPP
#define MEMORY_HPP
#include <cstdint>
#include <ostream>

class Memory {
public:
	Memory(const char* memory, uint64_t size);
   ~Memory();

	char& operator[](uint64_t index);
	Memory& operator=(const Memory& other);
	void operator+=(const char other);
	void operator-=(const uint64_t index);

	friend std::ostream& operator<<(std::ostream& os, const Memory& mem);


private:
	char* memory;
	uint64_t size;


};

#endif // !MEMORY_HPP
