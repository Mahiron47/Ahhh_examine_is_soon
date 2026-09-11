#include <iostream>
#include <limits>
#include "DynamicLib.hpp"

int main() {
    constexpr uint32_t SIZE_X = 24;
    constexpr uint32_t SIZE_Y = 24;

    Matrix2 matrix = Matrix2(SIZE_X, SIZE_Y);

    for (uint32_t x = 0; x < SIZE_X; ++x) {
        for (uint32_t y = 0; y < SIZE_Y; ++y) {
            matrix.set(Imatrix::Element{ .symbol = (get_random(0, 99) < 20) ? '%' : '-', .condition = 0, .info = 0 }, x, y);
    }   }

    matrix.print([](Imatrix::Element elem) -> std::string {
        if (elem.condition & Conditions::POINT_A_BIT) return "A";
        if (elem.condition & Conditions::POINT_B_BIT) return "B";

        return std::string(1, elem.symbol);
    });

    uint32_t result = 0;
    uint32_t max_sector_length = 0;
    
    while (true) {
        uint32_t ax = 0, ay = 0, bx = 0, by = 0;
        
        std::cout << "Enter 4 integers (coordinates Ax Ay Bx By from 1 to " << SIZE_X << "): ";
        
        if (!(std::cin >> ax >> ay >> bx >> by)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input error! Enter integers." << '\n';
            continue;
        }

        WaveAlgorithm algorithm = WaveAlgorithm(&matrix, 
                                                new Pos2(ax - 1, ay - 1), 
                                                new Pos2(bx - 1, by - 1), 
                                                DEFAULT_ORDER);

        result = algorithm.initiate();
        max_sector_length = std::to_string(algorithm.get_path_length()).length() - 1;

        switch (result) {
        case Results::SUCCESS:
            break;
        case Results::PATH_NOT_FOUND:
            break;
        case Results::INVALID_INPUT:
            continue;
        default:
            std::cout << "Unknown error." << '\n';
        }

        break;
    }

    std::cout << '\n' << "Discrete working field with points A and B:" << "\n\n";

    matrix.print([max_sector_length](Imatrix::Element elem) -> std::string {
        if (elem.condition & Conditions::POINT_A_BIT) return "A" + std::string(max_sector_length, ' ');
        if (elem.condition & Conditions::POINT_B_BIT) return "B" + std::string(max_sector_length, ' ');
        if (elem.info != 0) return std::to_string(elem.info) + std::string(max_sector_length - std::to_string(elem.info).length() + 1, ' ');

        return std::string(1, elem.symbol) + std::string(max_sector_length, ' ');
    });

    return 0;
}