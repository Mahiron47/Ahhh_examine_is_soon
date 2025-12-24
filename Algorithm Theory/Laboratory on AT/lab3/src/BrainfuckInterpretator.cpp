#include "BrainfuckInterpretator.hpp"
#include "logger/Logger.hpp"

#include <iostream>
#include <windows.h>
#include <fcntl.h>
#include <io.h>

Pointer BrainfuckInterpretator::code_pointer = Pointer();

void BrainfuckInterpretator::init(int argc, char* argv[]) {
	uint32_t file_path_index = 0;
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
				if (i + 1 < argc) {
                	is_input_file_path_defined = true;
					file_path_index = i + 1;
				}
                break;
            }}
        }
    }
    uint64_t code_size = 0;

    if (Logger::isDebug())
        Logger::log("Logger initialized successfully.");

    if (!is_input_file_path_defined) {
		if (Logger::isDebug()) {
        	Logger::error(ErrorCode::INPUT_FILE_UNDEFINED);
			throw ErrorCode(ErrorCode::INPUT_FILE_UNDEFINED);
        } else
			throw ErrorCode(ErrorCode::INPUT_FILE_UNDEFINED);
    }

	if (Logger::isDebug()) {
		Logger::log(std::string("Try to open file : ") + argv[file_path_index] + std::string("."));
	}

	FILE* f = nullptr;
	int err = fopen_s(&f, argv[file_path_index], "rb");
	
	if (err != 0 || f == nullptr) {
		if (Logger::isDebug()) {
			Logger::error(ErrorCode::INVALID_FILE_PATH);
			throw ErrorCode(ErrorCode::INVALID_FILE_PATH);
		} else 
			throw ErrorCode(ErrorCode::INVALID_FILE_PATH);
	}

    fseek(f, 0, SEEK_END);
    code_size = ftell(f);
    rewind(f);
    char* source_code = new char[code_size + 1];
    fread(source_code, sizeof(char), code_size, f);
    source_code[code_size] = '\0';
	
    fclose(f);

	BrainfuckInterpretator::code_pointer = Pointer(code_size);
	
	this->doMemoryInitialization(source_code);
    this->doInterpretation(source_code);

	delete[] source_code;
}

void BrainfuckInterpretator::init(const char* source_code, uint64_t code_size) {
	BrainfuckInterpretator::code_pointer = Pointer(code_size);
	
	this->doMemoryInitialization(source_code);
	this->doInterpretation(source_code);
}

void BrainfuckInterpretator::exec() {
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	
	std::string output = exe.str();
	std::cout << output;
	std::cout.flush();
}

uint64_t BrainfuckInterpretator::get_mem_size() const {
    return memory->size();
}

BrainfuckInterpretator& BrainfuckInterpretator::operator=(const BrainfuckInterpretator& other) {
	if (this != &other) {
		this->memory = other.memory;
	} 
	return *this;
}

void BrainfuckInterpretator::doMemoryInitialization(const char* source_code) {
    int64_t mem_index = 0;
    int64_t upper_pick = 0;

    do {
        switch (source_code[(uint64_t)code_pointer]) {
        case '>':
            if (++mem_index > upper_pick)
                upper_pick++;
            break;
        case '<':
            if (--mem_index < 0) {
                goto MEMORY_STOP;
            }
            break;
        }
    } while (++code_pointer);
MEMORY_STOP:
    memory = new Memory(upper_pick + 1);

    if (Logger::isDebug() && (--mem_index < 0))
        Logger::warn(WarningCode::MEMORY_SIZE_FIXED,
                     std::string(". The memory pointer attempted to access negative memory indices. The memory was fixed at a size equal to ") +
                     std::to_string(this->memory->size()) + std::string(" bytes"));

    code_pointer = (uint64_t)0;
}

void BrainfuckInterpretator::doInterpretation(const char* source_code) {
	do {
		switch (source_code[(uint64_t) code_pointer]) {
		// Cell value increment
		case '+':
			*memory += (char) 1;
			break;
		// Cell value decrement
		case '-':
			*memory -= (char) 1;
			break;
		// Move pointer to the right cell if pointer is at the end move to the start
		case '>':
			(*memory) += (uint64_t) 1;
			break;
		// Move pointer to the left cell if pointer is at the start move to the end
		case '<':
			(*memory) -= (uint64_t) 1;
			break;
		// Initialize cell to 135
		case '|':
			*memory = (char) 135;
			break;
		// Initialize cell to 25
		case '*':
			*memory = (char) 25;
			break;
		// Output cells value from zero Point position to current Point position
		case '~':
			exe << *memory;
			break;
		// Copy value from cell at end Point position to other cells
		case '$': {
			*memory = (uint64_t) 0;
			do {
				*memory = (*memory)[memory->size() - 1];
			} while (++(*memory));
			*memory = memory->size() - 1;
			break;
		}
		// Ignore whitespace characters
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			break;
		// Stop source code symbol
		case '\0':
			return;
		default:
			if (Logger::isDebug())
				Logger::error(ErrorCode::UNDEFINED_INSTRUCTION, 
							  std::string(". Undefined instruction '") + source_code[(uint64_t) code_pointer] + 
							  std::string("' encountered"));
			else
				throw ErrorCode(ErrorCode::UNDEFINED_INSTRUCTION);
			break;
		}
	} while (++code_pointer);
}
