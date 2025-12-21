#include "BrainfuckInterpretator.hpp"
#include "logger/Logger.hpp"

#include <iostream>

Pointer BrainfuckInterpretator::code_pointer = Pointer();

void BrainfuckInterpretator::init(int argc, char* argv[]) {
	bool is_input_file_path_defined = false;
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
					Logger::init();
 				}
                break;
            }
            case 'i': {
				if (i + 1 < argc)
                	is_input_file_path_defined = true;
                break;
            }}
        }
    }
    uint64_t code_size = 0;

    if (Logger::isDebug())
        Logger::log("Debug mode enabled.");

    if (!is_input_file_path_defined) {
        Logger::error(ErrorCode::INPUT_FILE_UNDEFINED);
        throw ErrorCode::INPUT_FILE_UNDEFINED;
    }

	auto f = fopen("input.txt", "r");
    
	if (f == nullptr) {
        Logger::error(ErrorCode::INVALID_FILE_PATH);
		throw ErrorCode::INVALID_FILE_PATH;
	}

    fseek(f, 0, SEEK_END);
    code_size = ftell(f);
    rewind(f);
    char* source_code = new char[code_size + 1];
    fread(source_code, sizeof(char), code_size, f);
    source_code[code_size] = '\0';
	
    fclose(f);
	
    this->doInterpretation(source_code);
	BrainfuckInterpretator::code_pointer = Pointer(code_size);
}

void BrainfuckInterpretator::exec() {
	
}

BrainfuckInterpretator& BrainfuckInterpretator::operator=(const BrainfuckInterpretator& other) {
	if (this != &other) {
		this->memory = other.memory;
	} 
	return *this;
}

void BrainfuckInterpretator::doInterpretation(const char* source_code) {
	Pointer mem_pointer = Pointer();

	while ((bool)code_pointer) {
		// TODO: Read next symbol from source code
		switch (source_code[(uint64_t)code_pointer]) {
		// Cell value increment
		case '+':
			++((char&)memory[(uint64_t)mem_pointer]);
			break;
		// Cell value decrement
		case '-':
			--((char&)memory[(uint64_t)mem_pointer]);
			break;
		// Move pointer to the right cell if pointer is at the end move to the start
		case '>':
			mem_pointer++;
			break;
		// Move pointer to the left cell if pointer is at the start move to the end
		case '<':
			mem_pointer--;
			break;
		// Initialize cell to 135
		case '|':
			memory += 135;
			mem_pointer + 1;
			break;
		// Initialize cell to 25
		case '*':
			memory += 25;
			mem_pointer + 1;
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
	code_pointer++;
}
