#include "DynamicLib.hpp"

int main() {
    constexpr uint32_t SIZE_X = 24;
    constexpr uint32_t SIZE_Y = 24;

    Matrix2 matrix(SIZE_X, SIZE_Y);

    for (uint32_t x = 0; x < SIZE_X; ++x) {
        for (uint32_t y = 0; y < SIZE_Y; ++y) {
            matrix.set(Imatrix::Element{ .symbol = (get_random(0, 99) < 20) ? '%' : '-', 
                                         .condition = 0, 
                                         .info = 0 }, 
                       x, y);
        }
    }

    std::cout << "Discrete working field (24x24):\n\n";
    matrix.print([](Imatrix::Element elem) -> std::string {
        return std::string(1, elem.symbol);
    });

    return 0;
}