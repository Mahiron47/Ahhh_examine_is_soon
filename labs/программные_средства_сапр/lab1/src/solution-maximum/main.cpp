#include <filesystem>
#include <fstream>
#include <limits>

#include "DynamicLib.hpp"

int main() {
    constexpr uint32_t SIZE_X = 24;
    constexpr uint32_t SIZE_Y = 24;

    Matrix2 matrix = Matrix2(SIZE_X, SIZE_Y);

    std::string filename;
    std::cout << "Enter DRP map file path: ";
    std::getline(std::cin, filename);

    std::filesystem::path full_path;
    std::filesystem::path current_dir = std::filesystem::current_path().parent_path().parent_path().parent_path() / L"labs" 
                                        / L"программные_средства_сапр" / L"lab1" / L"src" / L"solution-maximum" / L"assets";
    
    if (filename.empty()) {
        full_path = current_dir / L"default.txt";
    } else {
        full_path = current_dir / filename;
    }

    std::ifstream file(full_path);

    if (!file.is_open()) std::runtime_error("Failed to open file: " + full_path.string());

    for (uint32_t y = 0; y < SIZE_Y; ++y) {
        for (uint32_t x = 0; x < SIZE_X; ++x) {
            char ch = '-';

            if (!(file >> ch)) {
                ch = '-';
            }
            char sym = (ch == '%') ? '%' : '-';
    
            matrix.set(Imatrix::Element{ .symbol = sym, 
                                         .condition = 0, 
                                         .info = 0 }, 
                       x, y);
        }
    }
    file.close();

    std::cout << '\n' << "Loaded discrete working field (24x24):" << "\n\n";
    
    matrix.print([](Imatrix::Element elem) -> std::string {
        return std::string(1, elem.symbol);
    });

    uint32_t ax = 0, ay = 0, bx = 0, by = 0;
    uint32_t result = 0;
    uint32_t max_sector_length = 0;

    while (true) {
        std::cout << '\n' << "Enter 4 integers (coordinates Ax Ay Bx By from 1 to " << SIZE_X << "): ";

        if (!(std::cin >> ax >> ay >> bx >> by)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input error! Please enter integers.\n";
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
            std::cout << "Unknown error.\n";
        }

        break;
    }

    std::cout << '\n' << "Discrete working field with wave propagation (points A and B):" << "\n\n";

    matrix.print([max_sector_length](Imatrix::Element elem) -> std::string {
        if (elem.condition & Conditions::POINT_A_BIT) return "A" + std::string(max_sector_length, ' ');
        if (elem.condition & Conditions::POINT_B_BIT) return "B" + std::string(max_sector_length, ' ');
        if (elem.info != 0) return std::to_string(elem.info) + std::string(max_sector_length - std::to_string(elem.info).length() + 1, ' ');

        return std::string(1, elem.symbol) + std::string(max_sector_length, ' ');
    });

    if (result == Results::PATH_NOT_FOUND) return 0;

    std::cout << '\n' << "Discrete working field with the shortest path from A to B (#):" << "\n\n";

    matrix.print([](Imatrix::Element elem) -> std::string {
        if (elem.condition & Conditions::POINT_A_BIT) return "A";
        if (elem.condition & Conditions::POINT_B_BIT) return "B";
        if (elem.condition & Conditions::MARK_AS_PATH_BIT) return "#";

        return std::string(1, elem.symbol);
    });

    return 0;
}