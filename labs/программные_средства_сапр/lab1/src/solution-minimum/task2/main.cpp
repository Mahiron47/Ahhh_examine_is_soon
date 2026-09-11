#include <limits>

#include "DynamicLib.hpp"

int main() {
    constexpr uint32_t SIZE_X = 24;
    constexpr uint32_t SIZE_Y = 24;

    Matrix2 matrix(SIZE_X, SIZE_Y);

    for (uint32_t x = 0; x < SIZE_X; ++x) {
        for (uint32_t y = 0; y < SIZE_Y; ++y) {
            char symbol = (get_random(0, 99) < 20) ? '%' : '-';
            matrix.set(Imatrix::Element{ .symbol = symbol, .condition = 0, .info = 0 }, x, y);
    }   }

    Pos2 point_a, point_b;

    while (true) {
        uint32_t ax = 0, ay = 0, bx = 0, by = 0;
        
        std::cout << "Enter 4 integers (coordinates Ax Ay Bx By from 0 to " << SIZE_X << "): ";
        
        if (!(std::cin >> ax >> ay >> bx >> by)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input error! Enter integers." << '\n';
            continue;
        }

        point_a = Pos2(ax - 1, ay - 1);
        point_b = Pos2(bx - 1, by - 1);

        if (point_a.x >= SIZE_X || point_a.y >= SIZE_Y || point_b.x >= SIZE_X || point_b.y >= SIZE_Y || (point_a == point_b)) {
            std::cout << "Coordinates are out of bounds [0, " << SIZE_X << "]! Please try again." << '\n';
            continue;
        }


        break;
    }


    matrix.set(Imatrix::Element { .symbol = 'A', 
                                  .condition = Conditions::POINT_A_BIT, 
                                  .info = 0 }, 
               point_a);
    matrix.set(Imatrix::Element { .symbol = 'B', 
                                  .condition = Conditions::POINT_B_BIT, 
                                  .info = 0 }, 
               point_b);

    std::cout << '\n' << "Discrete working field with points A and B:" << "\n\n";

    matrix.print([](Imatrix::Element elem) -> std::string {
        return std::string(1, elem.symbol);
    });

    return 0;
}