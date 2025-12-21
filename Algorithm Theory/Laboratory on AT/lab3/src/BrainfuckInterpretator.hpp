#ifndef BRAINFUCKINTERPRITATOR_HPP
#define BRAINFUCKINTERPRITATOR_HPP
#include "memory/Memory.hpp"
#include "memory/Pointer.hpp"

class BrainfuckInterpretator {
	Memory* memory;

	static Pointer code_pointer;

	void doInterpretation(const char* source_code);
	
	
public:
	static uint64_t getCodePosition() {
		return (uint64_t)code_pointer;
	}

	BrainfuckInterpretator() = default;
   ~BrainfuckInterpretator() = default;
	
    void init(int argc, char* argv[]);
	void exec();

	BrainfuckInterpretator& operator=(const BrainfuckInterpretator& other);
};

#endif // !BRAINFUCKINTERPRITATOR_HPP
