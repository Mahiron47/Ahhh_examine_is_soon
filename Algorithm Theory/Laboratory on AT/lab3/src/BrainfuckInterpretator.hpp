#ifndef BRAINFUCKINTERPRITATOR_HPP
#define BRAINFUCKINTERPRITATOR_HPP
#include "memory/Memory.hpp"
#include "memory/Pointer.hpp"

#include <sstream>

class BrainfuckInterpretator {
    Memory* memory;
	std::ostringstream exe;

	static Pointer code_pointer;
	
	void doMemoryInitialization(const char* source_code);
	void doInterpretation(const char* source_code);

public:
    static uint64_t getCodePosition() {
        return (uint64_t)code_pointer;
    }
	
	BrainfuckInterpretator() = default;
    ~BrainfuckInterpretator() = default;

    void init(int argc, char* argv[]);
	void init(const char* source_code, uint64_t code_size);
    void exec();
    
    uint64_t get_mem_size() const;

    BrainfuckInterpretator& operator=(const BrainfuckInterpretator& other);
};

#endif // !BRAINFUCKINTERPRITATOR_HPP
