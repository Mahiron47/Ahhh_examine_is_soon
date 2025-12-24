#include "BrainfuckInterpretator.hpp"
#include "memory/Memory.hpp"

#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
	BrainfuckInterpretator interpreter = BrainfuckInterpretator();
	
	std::vector<const char*> arg = {"-DEBUG", "-i", "C:\\Users\\Admin\\Desktop\\Ahhh_examine_is_soon\\Algorithm Theory\\Laboratory on AT\\lab3\\res\\Task2ru.bf"};
	interpreter.init((int) arg.size(), const_cast<char**>(arg.data()));
	
	interpreter.exec();

	//BrainfuckInterpretator interpreter;
    //std::string code = "|------------->*++++++++>*++++++++>*|------------->*++++++++>|-------------~";
	//Logger::init();
    //interpreter.init(code.c_str(), code.size());
    //interpreter.exec();
	return 0;
}