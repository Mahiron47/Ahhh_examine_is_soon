#include "BrainfuckInterpretator.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
	BrainfuckInterpretator* interpretator = new BrainfuckInterpretator();
	interpretator->init(argc, argv);

	Memory mem1("abcdefghij", 10);
	Memory mem2("1111111111", 10);

	std::cout << mem1 << std::endl;
	std::cout << mem2 << std::endl;
	mem2 = mem1;
	std::cout << mem1 << std::endl;
	std::cout << mem2 << std::endl;

	delete interpretator;

	return 0;
}