#ifndef BRAINFUCKINTERPRITATOR_HPP
#define BRAINFUCKINTERPRITATOR_HPP
#include "Memory.hpp"
#include "Pointer.hpp"

class BrainfuckInterpretator {
public:
	BrainfuckInterpretator() = default;
   ~BrainfuckInterpretator() = default;
	
    void init(int argc, char* argv[]);
	void exec();

	BrainfuckInterpretator& operator=(const BrainfuckInterpretator& other);
private:
	Memory* memory;

	void doInterpretation(const char* source_code, Pointer& pointer);
};

#endif // !BRAINFUCKINTERPRITATOR_HPP
