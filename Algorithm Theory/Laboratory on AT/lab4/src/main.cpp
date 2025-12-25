#include "Life.hpp"

int main(int argc, char** argv){
    //15x15.txt glider pattern from https://conwaylife.com/wiki/Glider
	Life life(15);
    life.init(argc, argv);
    life.exec();
}
