#include <filesystem>
#include <fstream>
#include <limits>

#include "DynamicLib.hpp"

int main() {
    constexpr uint32_t SIZE_Z = 2;

    Matrix3 matrix;
    Pos2 shift;
    Pos2 shift_plus_size;

    Pos3 pos_a;
    Pos3 pos_b;

    std::ifstream file;

    auto func_z_layer_convert_to_int = [](std::string MZ) -> uint32_t {
        return static_cast<uint32_t>(MZ[2] - '0');
    };

    /* open the file */ {
        std::string filename;
        std::cout << "Enter DRP code file path: ";
        std::getline(std::cin, filename);

        std::filesystem::path full_path;
        std::filesystem::path current_dir = std::filesystem::current_path().parent_path().parent_path().parent_path() / L"labs" 
                                            / L"программные_средства_сапр" / L"lab1" / L"src" / L"solution-individual" / L"assets";

        if (filename.empty()) {
            full_path = current_dir / L"default.txt";
        } else {
            full_path = current_dir / filename;
        }

        file = std::ifstream(full_path);

        if (!file.is_open()) std::runtime_error("Failed to open file: " + full_path.string());
    } /* open the file */

    /* read the file */ {
        while (true) {
            std::string command;
        
            if (!(file >> command)) break; 

            switch (command[1]) {
            case 'B':
                if (command == "BBOX") {
                    if (!(file >> shift.x >> shift.y >> shift_plus_size.x >> shift_plus_size.y)) throw std::runtime_error("Invalid BBOX command format.");
                    
                    matrix = Matrix3(shift_plus_size.x - shift.x, shift_plus_size.y - shift.y, SIZE_Z); // TODO: correct constructor
                    
                    for (uint32_t z = 0; z < SIZE_Z; z++) {
                        for (uint32_t x = 0; x < shift_plus_size.x - shift.x; x++) {
                            for (uint32_t y = 0; y < shift_plus_size.y - shift.y; y++) {
                                matrix.set(Imatrix::Element{ .symbol = '-',
                                                             .condition = 0,
                                                             .info = 0 },
                                           Pos3(x, y, z));
                }   }   }   } else {
                    throw std::runtime_error("Unknown command: " + command);
                }
                continue;
            case 'G':
                if (command == "BGN") {
                    std::string mz;

                    if (!(file >> pos_a.x >> pos_a.y >> mz)) throw std::runtime_error("Invalid BGN command format.");
                    if (shift > pos_a || pos_a >= shift_plus_size) throw std::runtime_error("Point A is out of bounds.");
                    
                    pos_a.z = func_z_layer_convert_to_int(mz);

                    matrix.set(Imatrix::Element { .symbol = 'A',
                                                  .condition = Conditions::POINT_A_BIT,
                                                  .info = 0 },
                               pos_a - shift);
                }
                continue;
            case 'N':
                if (command == "END") {
                    std::string mz;

                    if (!(file >> pos_b.x >> pos_b.y >> mz)) throw std::runtime_error("Invalid END command format.");
                    if (shift > pos_b || pos_b >= shift_plus_size) throw std::runtime_error("Point B is out of bounds.");

                    pos_b.z = func_z_layer_convert_to_int(mz);

                    matrix.set(Imatrix::Element { .symbol = 'B',
                                                  .condition = Conditions::POINT_B_BIT,
                                                  .info = 0 },
                               pos_b - shift);
                    break;
                } else {
                    throw std::runtime_error("Unknown command: " + command);
                }
                continue;
            case 'E':
                if (command == "REC") {
                    Pos3 static_point;
                    Pos2 size;
                    std::string mz;
                    
                    if (!(file >> static_point.x >> static_point.y >> size.x >> size.y >> mz)) throw std::runtime_error("Invalid REC command format.");
                    if (shift >= static_point || static_point + Pos3(size.x, size.y, 0) > shift_plus_size) throw std::runtime_error("Rectangle is out of bounds.");
                    
                    static_point.z = func_z_layer_convert_to_int(mz);

                    for (uint32_t z = static_point.z; z < static_point.z + SIZE_Z; ++z) {
                        for (uint32_t y = static_point.y; y < static_point.y + size.y; ++y) {
                            for (uint32_t x = static_point.x; x < static_point.x + size.x; ++x) {
                                matrix.set(Imatrix::Element { .symbol = '%',
                                                              .condition = 0,
                                                              .info = 0 },
                                           Pos3(x, y, z) - shift);
                }   }   }   } else {
                    throw std::runtime_error("Unknown command: " + command);
                } 
                continue;
            default:
                throw std::runtime_error("Unknown command: " + command);  
        }   }
    } /* read the file */

    /* close the file */ {
        file.close();
    } /* close the file */

    /* print the loaded matrix */ {
        std::cout << '\n'
                  << "Loaded discrete working field (3 layers):" << "\n\n";

        matrix.print([](Imatrix::Element elem) -> std::string {
            return std::string(1, elem.symbol);
        });
    } /* print the loaded matrix */
    
    uint32_t result = 0;
    uint32_t max_sector_length = 0;

    WaveAlgorithm algorithm = WaveAlgorithm(&matrix,
                                            new Pos3(pos_a),
                                            new Pos3(pos_b),
                                            DEFAULT_ORDER);

    result = algorithm.initiate();
    max_sector_length = std::to_string(algorithm.get_path_length()).length() - 1;

    if (result == Results::INVALID_INPUT) return 0;

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