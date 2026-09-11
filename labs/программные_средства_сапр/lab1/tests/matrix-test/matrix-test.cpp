#include <catch2/catch_all.hpp>

#include "DynamicLib.hpp"

TEST_CASE("Matrix2::Matrix2(Args&&...)", "[Matrix2], [constructors], [get]") {
    SECTION("Matrix2::Matrix2(size) : simple check") {
        Matrix2 matrix = Matrix2(10, 10);
        REQUIRE(matrix.get_size().x == 10);
        REQUIRE(matrix.get_size().y == 10);
    }

    SECTION("Matrix2::Matrix2(Args&&...) : simple check") {
        Matrix2 matrix = Matrix2(10, 10,
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', 'A', 'B', 'C', 'D', 'E', 'F', 'G', '-',
            '-', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', '-',
            '-', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', '-',
            '-', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', '-',
            '-', '`', '{', '|', '}', '~', '\0','\0','\0','-',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','-'
        );

        REQUIRE(matrix.get_size().x == 10);
        REQUIRE(matrix.get_size().y == 10);

        REQUIRE(matrix.get(4, 6).symbol == 'S');
    }
}

TEST_CASE("Matrix2::set", "[Matrix2], [set], [get]") {
    SECTION("Matrix2::set ; Matrix2::get : simple check") {
        Matrix2 matrix = Matrix2(10, 10);
        matrix.set(Imatrix::Element { .symbol = 'A', 
                                      .condition = 0,
                                      .info = 0 }, 
                   Pos2(4, 6));
        REQUIRE(matrix.get(4, 6).symbol == 'A');
        Imatrix::Element elem = matrix.get(4, 6);
        matrix.set(Imatrix::Element { .symbol = 'B', 
                                      .condition = elem.condition | Conditions::CHECKED_BIT,
                                      .info = elem.info }, 
                   Pos2(4, 6));
        REQUIRE(matrix.get(4, 6).symbol == 'B');
        REQUIRE(matrix.get(4, 6).condition & Conditions::CHECKED_BIT);
    }
}

TEST_CASE("Matrix3::Matrix3(Args&&...)", "[Matrix3], [constructors], [get]") {
    SECTION("Matrix3::Matrix3(size) : simple check") {
        Matrix3 matrix = Matrix3(10, 10, 10);
        REQUIRE(matrix.get_size().x == 10);
        REQUIRE(matrix.get_size().y == 10);
        REQUIRE(matrix.get_size().z == 10);
    }

    SECTION("Matrix3::Matrix3(Args&&...) : simple check") {
        Matrix3 matrix = Matrix3(10, 10, 3,
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', 'A', 'B', 'C', 'D', 'E', 'F', 'G', '-',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','-',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','-',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','-',
            '\0','\0','\0','C','\0','\0','\0','\0','\0','-',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','-',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0', '-',
            
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', 'A', 'B', 'C', 'D', 'E', 'F', 'G', '-',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','-',
            '\0','\0','L','\0','\0','\0','\0','\0','\0','-',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','-',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','-',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','-',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0', '-',
            
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', 'A', 'B', 'C', 'D', 'E', 'F', 'G', '-',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','-',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','-',
            '\0','\0','\0','\0','\0','\0','\0','K','\0','-',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','-',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','-',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0', '-'
        );

        REQUIRE(matrix.get_size().x == 10);
        REQUIRE(matrix.get_size().y == 10);
        REQUIRE(matrix.get_size().z == 3);

        REQUIRE(matrix.get(2, 3, 0).symbol == 'A');
        REQUIRE(matrix.get(2, 3, 1).symbol == 'A');
        REQUIRE(matrix.get(2, 3, 2).symbol == 'A');

        REQUIRE(matrix.get(3, 7, 0).symbol == 'C');
        REQUIRE(matrix.get(2, 5, 1).symbol == 'L');
        REQUIRE(matrix.get(7, 6, 2).symbol == 'K');
    }
}

TEST_CASE("Matrix3::set", "[Matrix3], [set], [get]") {
    SECTION("Matrix3::set ; Matrix3::get : simple check") {
        Matrix3 matrix = Matrix3(10, 10, 3);
        REQUIRE(matrix.get(4, 6, 1).symbol != 'A');
        matrix.set(Imatrix::Element { .symbol = 'A', 
                                      .condition = 0, 
                                      .info = 0 }, 
                   Pos3(4, 6, 1));
        REQUIRE(matrix.get(4, 6, 1).symbol == 'A');
        Imatrix::Element elem = matrix.get(4, 6, 1);
        matrix.set(Imatrix::Element { .symbol = 'B', 
                                      .condition = elem.condition | Conditions::CHECKED_BIT,
                                      .info = elem.info }, 
                   Pos3(4, 6, 1));
        REQUIRE(matrix.get(4, 6, 1).condition & Conditions::CHECKED_BIT);
        REQUIRE(matrix.get(4, 6, 1).symbol == 'B');
    }

    SECTION("Matrix3 with non-symmetric dimensions (size_x != size_y)") {
        // Dimensions 11 x 14 x 2, matching solution-individual BBOX
        constexpr uint32_t SX = 11;
        constexpr uint32_t SY = 14;
        constexpr uint32_t SZ = 2;

        Matrix3 matrix(SX, SY, SZ);
        REQUIRE(matrix.get_size().x == SX);
        REQUIRE(matrix.get_size().y == SY);
        REQUIRE(matrix.get_size().z == SZ);

        // Fill entire matrix and check every cell
        for (uint32_t z = 0; z < SZ; ++z) {
            for (uint32_t y = 0; y < SY; ++y) {
                for (uint32_t x = 0; x < SX; ++x) {
                    matrix.set(Imatrix::Element{ .symbol = '-', .condition = 0, .info = z * 1000 + y * 10 + x }, x, y, z);
                }
            }
        }

        // Verify boundary and interior values
        REQUIRE(matrix.get(0, 0, 0).info == 0);
        REQUIRE(matrix.get(10, 13, 1).info == 1130 + 10);
        REQUIRE(matrix.get(10, 0, 0).info == 10);
        REQUIRE(matrix.get(0, 13, 0).info == 130);

        // Test assignment and move semantics (rvalue assignment like in solution-individual)
        Matrix3 assigned;
        assigned = Matrix3(SX, SY, SZ);
        assigned.set(Imatrix::Element{ .symbol = '%', .condition = 0, .info = 42 }, Pos3(10, 13, 1));
        REQUIRE(assigned.get(10, 13, 1).symbol == '%');
        REQUIRE(assigned.get(10, 13, 1).info == 42);

        // Copy constructor
        Matrix3 copied = assigned;
        REQUIRE(copied.get(10, 13, 1).symbol == '%');
    }
}