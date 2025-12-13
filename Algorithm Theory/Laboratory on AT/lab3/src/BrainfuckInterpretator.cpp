#include "BrainfuckInterpretator.hpp"

#include <iostream>

void BrainfuckInterpretator::init(int argc, char* argv[]) {
	bool is_input_file = false;
    for (int i = 0; i < argc; ++i) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
            case 'D': {
                uint32_t a = 0;
                a += argv[i][2] << 24;
                a += argv[i][3] << 16;
                a += argv[i][4] << 8;
                a += argv[i][5];
                if (a == 0x45425547) {

 				}
                break;
            }
            case 'i': {
				if (i + 1 < argc)
                	is_input_file = true;
                break;
            }}
        }
        std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
    }
    uint64_t code_size = 0;

#ifdef DEBUG
	std::cout << "Debug mode enabled" << std::endl;
#endif

    Pointer pointer(code_size);
	if (is_input_file) {
		auto f = fopen("input.txt", "r");
		if (f == nullptr) std::cerr << "Error opening file" << std::endl;
		
		fseek(f, 0, SEEK_END);
		code_size = ftell(f);
		rewind(f);
		char* source_code = new char[code_size + 1];
		fread(source_code, sizeof(char), code_size, f);
		source_code[code_size] = '\0';

		fclose(f);
		
    	doInterpretation(source_code, pointer);
	} else {
		std::cerr << "No input file specified" << std::endl;	
	}
}

void BrainfuckInterpretator::exec() {
	
}

BrainfuckInterpretator& BrainfuckInterpretator::operator=(const BrainfuckInterpretator& other) {
	if (this != &other) {
		this->memory = other.memory;
	} 
	return *this;
}

void BrainfuckInterpretator::doInterpretation(const char* source_code, Pointer& pointer) {
	while ((bool)pointer) {
		// TODO: Read next symbol from source code
		switch (source_code[(uint64_t)pointer]) {
		// Cell value increment
		case '+':

			break;
		// Cell value decrement
		case '-':

			break;
		// Move pointer to the right cell if pointer is at the end move to the start
		case '>':

			break;
		// Move pointer to the left cell if pointer is at the start move to the end
		case '<':

			break;
		// Initialize cell to 135
		case '|':

			break;
		// Initialize cell to 25
		case '*':

			break;
		// Output cells value from zero Point position to current Point position
		case '~':

			break;
		// Copy value from cell at end Point position to other cells
		case '$':

			break;
		// Stop source code symbol
		case '\0':
			return;
		// TODO: Implement error handling
		default:
			break;
		}
	}
	pointer++;
}
