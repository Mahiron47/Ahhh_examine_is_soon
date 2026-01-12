#include "Life.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <conio.h>
#include <iostream>

string Life::read_file(const string& file_path) {
	std::ifstream file(file_path);
	if (!file.is_open()) {
		throw "FILE_OPEN_ERROR";
	}
	
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	
	return buffer.str();
}

void Life::write_to_end(const string& text) {
	if (!output_file.is_open()) {
		throw "FILE_NOT_OPEN_ERROR";
	}
	output_file << text;
	output_file.flush();
}

string Life::iterate_generations(uint32_t count) {
	for (uint32_t gen = 0; gen < count; gen++) {
		dsc_matrix->do_each_cell([](bool cell, DSCposition pos, DiscreteMatrix& dsc_matrix) -> bool {
			DSCneighbors neighbors = dsc_matrix.get_neighbors(pos);
			
			uint8_t alive_neighbors = neighbors.top_left + neighbors.top + neighbors.top_right +
									  neighbors.left + neighbors.right +
									  neighbors.bottom_left + neighbors.bottom + neighbors.bottom_right;
			
			if (cell) return alive_neighbors == 2 || alive_neighbors == 3;
				 else return alive_neighbors == 3;
		});
	}

    return (string) dsc_matrix;
}

DSCmatrix Life::generate_matrix(const string& input_file) {
	std::vector<bool> cells;
	for (uint64_t i = 0; i < input_file.size(); i++) {
		if (input_file[i] == '1') {
			cells.push_back(true);
		} else if (input_file[i] == '0') {
			cells.push_back(false);
		}
	}
    return DiscreteMatrix::create(cells);
}

Life::Life(uint32_t size) : dsc_matrix{DiscreteMatrix::create(size, size)} {
}

Life::~Life() {
	DiscreteMatrix::destroy(dsc_matrix);
}

void Life::init(int argc, char** argv) {
    uint32_t file_path_index = 0;
	string input_file;
    for (int i = 0; i < argc; ++i) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
			case 'i':
                if (i + 1 < argc) {
                    input_file = read_file(argv[i + 1]);
                } else {
        			throw "INPUT_FILE_PATH_NOT_DEFINED";
    			}
                break;
            case 'o': 
                if (i + 1 < argc) {
                    output_file.open(argv[i + 1], std::ios::out | std::ios::trunc);
					if (!output_file.is_open()) {
						throw "FILE_OPEN_ERROR";
					}
                } else {
					throw "OUTPUT_FILE_PATH_NOT_DEFINED";
				}
                break;
            }
        }
    }

	if (!input_file.empty()) {
		dsc_matrix = generate_matrix(input_file);
	} else {
		throw "INPUT_FILE_IS_EMPTY";
	}


}

void Life::exec() {
	bool running = true;
	
	while (running) {
		if (_kbhit()) {
			char key = _getch();
			
			switch (key) {
			case 27: // ESC
				running = false;
				std::cout << "Exiting...\n";
				break;
			
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				std::cout << "Iterating " << (key - '0') << " generation(s)\n";
				write_to_end(iterate_generations(key - '0'));
				break;
			default:
				std::cout << "Unknown key: " << key << " (Press 0-9 for iterate generations, ESC to exit)\n";
				break;
			}
		}
	}

	if (output_file.is_open()) {
		output_file.close();
	}
}
