#include "BrainfuckInterpretator.hpp"
#include "memory/Memory.hpp"

int main(int argc, char* argv[]) {
	BrainfuckInterpretator interpreter = BrainfuckInterpretator();
	
	interpreter.init(argc, argv);
	interpreter.exec();

	return 0;
}