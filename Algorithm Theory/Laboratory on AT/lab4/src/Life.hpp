#ifndef LIFE_HPP
#define LIFE_HPP
#include "memory/DiscreteMatrix.hpp"

#include <fstream>

class Life {
protected:
	DSCmatrix dsc_matrix;
	std::ofstream output_file;

	string read_file(const string& file_path);
	void write_to_end(const string& text);

	string iterate_generations(uint32_t count);

	DSCmatrix generate_matrix(const string& input_file);

public:
	Life(uint32_t size);
	~Life();
	
	void init(int argc, char** argv);
	void exec();
};

#endif // LIFE_HPP