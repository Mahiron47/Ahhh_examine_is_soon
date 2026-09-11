#include <catch2/catch_all.hpp>

#include "DynamicLib.hpp"

TEST_CASE ("WaveAlgorithm::WaveAlgorithm", "[WaveAlgorithm]") {
    SECTION ("WaveAlgorithm::WaveAlgorithm : simple test heap matrix") {
        WaveAlgorithm(new Matrix2(10, 10), 
                      new Pos2(0, 0), 
                      new Pos2(9, 9), 
                      DEFAULT_ORDER, true);

        REQUIRE(true);
    }
    
    SECTION ("WaveAlgorithm::WaveAlgorithm : simple test stack matrix") {
        Matrix2 matrix = Matrix2(10, 10);
        
        WaveAlgorithm(&matrix, 
                      new Pos2(0, 0), 
                      new Pos2(9, 9), 
                      DEFAULT_ORDER);

        REQUIRE(true);
    }

}

TEST_CASE ("WaveAlgorithm::initiate", "[WaveAlgorithm]") {
    SECTION ("WaveAlgorithm::initiate : simple test") {
        WaveAlgorithm algorithm = WaveAlgorithm(new Matrix2(10, 10,
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'
        ), new Pos2(0, 0), new Pos2(9, 9), DEFAULT_ORDER, true);

        REQUIRE(algorithm.initiate() == Results::SUCCESS);
    }   

    SECTION ("WaveAlgorithm::initiate : simple test with obstacles") {
        WaveAlgorithm algorithm = WaveAlgorithm(new Matrix2(10, 10,
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '%', '%', '%', '%', '%', '%', '%', '%', '-',
            '-', '%', '-', '-', '-', '-', '-', '-', '%', '-',
            '-', '%', '-', '%', '%', '%', '%', '-', '%', '-',
            '-', '%', '-', '%', '-', '-', '%', '-', '%', '-',
            '-', '%', '-', '%', '-', '%', '%', '-', '%', '-',
            '-', '%', '-', '%', '-', '%', '-', '-', '%', '-',
            '-', '%', '-', '%', '-', '%', '%', '%', '%', '-',
            '-', '%', '-', '-', '-', '-', '-', '-', '%', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'
        ), new Pos2(5, 4), new Pos2(7, 6), DEFAULT_ORDER, true);

        REQUIRE(algorithm.initiate() == Results::SUCCESS);
    }

    SECTION ("WaveAlgorithm::initiate : simple test with obstacles and no path") {
        WaveAlgorithm algorithm = WaveAlgorithm(new Matrix2(10, 10,
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '%', '%', '%', '%', '%', '%', '%', '%', '-',
            '-', '%', '-', '-', '-', '-', '-', '-', '%', '-',
            '-', '%', '-', '%', '%', '%', '%', '-', '%', '-',
            '-', '%', '-', '%', '-', '-', '%', '-', '%', '-',
            '-', '%', '-', '%', '-', '-', '%', '-', '%', '-',
            '-', '%', '-', '%', '%', '%', '%', '-', '%', '-',
            '-', '%', '-', '%', '-', '-', '-', '-', '%', '-',
            '-', '%', '-', '-', '-', '-', '-', '-', '%', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'
        ), new Pos2(2, 8), new Pos2(5, 5), DEFAULT_ORDER, true);

        REQUIRE(algorithm.initiate() == Results::PATH_NOT_FOUND);
    }

    SECTION ("WaveAlgorithm::initiate : simple test") {
        WaveAlgorithm algorithm = WaveAlgorithm(new Matrix2(10, 10,
            'A', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', 'B'
        ), DEFAULT_ORDER, true);

        REQUIRE(algorithm.initiate() == Results::SUCCESS);
    }   

    SECTION ("WaveAlgorithm::initiate : simple test with obstacles") {
        WaveAlgorithm algorithm = WaveAlgorithm(new Matrix2(10, 10,
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '%', '%', '%', '%', '%', '%', '%', '%', '-',
            '-', '%', '-', '-', '-', '-', '-', '-', '%', '-',
            '-', '%', '-', '%', '%', '%', '%', '-', '%', '-',
            '-', '%', '-', '%', '-', 'A', '%', '-', '%', '-',
            '-', '%', '-', '%', '-', '%', '%', '-', '%', '-',
            '-', '%', '-', '%', '-', '%', '-', 'B', '%', '-',
            '-', '%', '-', '%', '-', '%', '%', '%', '%', '-',
            '-', '%', '-', '-', '-', '-', '-', '-', '%', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'
        ), DEFAULT_ORDER, true);

        REQUIRE(algorithm.initiate() == Results::SUCCESS);
    }

    SECTION ("WaveAlgorithm::initiate : simple test with obstacles and no path") {
        WaveAlgorithm algorithm = WaveAlgorithm(new Matrix2(10, 10,
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '%', '%', '%', '%', '%', '%', '%', '%', '-',
            '-', '%', '-', '-', '-', '-', '-', '-', '%', '-',
            '-', '%', '-', '%', '%', '%', '%', '-', '%', '-',
            '-', '%', '-', '%', '-', '-', '%', '-', '%', '-',
            '-', '%', '-', '%', '-', 'B', '%', '-', '%', '-',
            '-', '%', '-', '%', '%', '%', '%', '-', '%', '-',
            '-', '%', '-', '%', '-', '-', '-', '-', '%', '-',
            '-', '%', 'A', '-', '-', '-', '-', '-', '%', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'
        ), DEFAULT_ORDER, true);

        REQUIRE(algorithm.initiate() == Results::PATH_NOT_FOUND);
    }

    SECTION ("WaveAlgorithm::initiate : simple test") {
        WaveAlgorithm algorithm = WaveAlgorithm(new Matrix2(10, 10,
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'
        ), new Pos2(0, 0), new Pos2(10, 9), DEFAULT_ORDER, true);

        REQUIRE(algorithm.initiate() == Results::INVALID_INPUT);
    }

    SECTION ("WaveAlgorithm::initiate : simple test") {
        WaveAlgorithm algorithm = WaveAlgorithm(new Matrix2(10, 10,
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
            '-', '-', '-', '-', '-', '-', '-', '-', '-', '%'
        ), new Pos2(0, 0), new Pos2(9, 9), DEFAULT_ORDER, true);

        REQUIRE(algorithm.initiate() == Results::INVALID_INPUT);
    }
}

TEST_CASE ("WaveAlgorithm::WaveAlgorithm (3D)", "[WaveAlgorithm][3D]") {
    SECTION ("WaveAlgorithm::WaveAlgorithm : 3D simple test heap matrix") {
        WaveAlgorithm(new Matrix3(3, 3, 3), 
                      new Pos3(0, 0, 0), 
                      new Pos3(2, 2, 2), 
                      DEFAULT_ORDER);

        REQUIRE(true);
    }

    SECTION ("WaveAlgorithm::WaveAlgorithm : 3D simple test stack matrix") {
        Matrix3 matrix = Matrix3(3, 3, 3);
        
        WaveAlgorithm(&matrix, 
                      new Pos3(0, 0, 0), 
                      new Pos3(2, 2, 2), 
                      DEFAULT_ORDER);

        REQUIRE(true);
    }
}

TEST_CASE ("WaveAlgorithm::initiate (3D)", "[WaveAlgorithm][3D]") {
    SECTION ("WaveAlgorithm::initiate : 3D simple test") {
        WaveAlgorithm algorithm = WaveAlgorithm(new Matrix3(3, 3, 3,
            // Слой z = 0
            '-', '-', '-',
            '-', '-', '-',
            '-', '-', '-',

            // Слой z = 1
            '-', '-', '-',
            '-', '-', '-',
            '-', '-', '-',

            // Слой z = 2
            '-', '-', '-',
            '-', '-', '-',
            '-', '-', '-'
        ), new Pos3(0, 0, 0), new Pos3(2, 2, 2), DEFAULT_ORDER, true);

        REQUIRE(algorithm.initiate() == Results::SUCCESS);
    }   

    SECTION ("WaveAlgorithm::initiate : 3D simple test with obstacles") {
        // Путь между слоями z=0 и z=2 существует только через один проход (2, 2, 1) в слое z=1
        WaveAlgorithm algorithm = WaveAlgorithm(new Matrix3(3, 3, 3,
            // Слой z = 0: Начало пути в (0, 0, 0)
            '-', '-', '-',
            '-', '-', '-',
            '-', '-', '-',

            // Слой z = 1: Препятствия, открыта только ячейка (2, 2, 1)
            '%', '%', '%',
            '%', '%', '%',
            '%', '%', '-',

            // Слой z = 2: Конец пути в (0, 0, 2)
            '-', '-', '-',
            '-', '-', '-',
            '-', '-', '-'
        ), new Pos3(0, 0, 0), new Pos3(0, 0, 2), DEFAULT_ORDER, true);

        REQUIRE(algorithm.initiate() == Results::SUCCESS);
    }

    SECTION ("WaveAlgorithm::initiate : 3D simple test with obstacles and no path") {
        // Слой z = 1 полностью заблокирован препятствиями '%' — путь на слой z=2 невозможен
        WaveAlgorithm algorithm = WaveAlgorithm(new Matrix3(3, 3, 3,
            // Слой z = 0
            '-', '-', '-',
            '-', '-', '-',
            '-', '-', '-',

            // Слой z = 1 (глухая стена)
            '%', '%', '%',
            '%', '%', '%',
            '%', '%', '%',

            // Слой z = 2
            '-', '-', '-',
            '-', '-', '-',
            '-', '-', '-'
        ), new Pos3(0, 0, 0), new Pos3(2, 2, 2), DEFAULT_ORDER, true);

        REQUIRE(algorithm.initiate() == Results::PATH_NOT_FOUND);
    }

    SECTION ("WaveAlgorithm::initiate : 3D auto-detect 'A' and 'B'") {
        // Точки А и B заданы прямо в матрице
        WaveAlgorithm algorithm = WaveAlgorithm(new Matrix3(3, 3, 3,
            // Слой z = 0: Точка 'A' в (0, 0, 0)
            'A', '-', '-',
            '-', '-', '-',
            '-', '-', '-',

            // Слой z = 1
            '-', '-', '-',
            '-', '-', '-',
            '-', '-', '-',

            // Слой z = 2: Точка 'B' в (2, 2, 2)
            '-', '-', '-',
            '-', '-', '-',
            '-', '-', 'B'
        ), DEFAULT_ORDER, true);

        REQUIRE(algorithm.initiate() == Results::SUCCESS);
    }

    SECTION ("WaveAlgorithm::initiate : 3D auto-detect with obstacles and no path") {
        // Точка 'B' изолирована со всех сторон стенами '%'
        WaveAlgorithm algorithm = WaveAlgorithm(new Matrix3(3, 3, 3,
            // Слой z = 0: 'A' в (0, 0, 0)
            'A', '-', '-',
            '-', '-', '-',
            '-', '-', '-',

            // Слой z = 1: стена
            '%', '%', '%',
            '%', '%', '%',
            '%', '%', '%',

            // Слой z = 2: 'B' за стеной
            '-', '-', '-',
            '-', '-', '-',
            '-', '-', 'B'
        ), DEFAULT_ORDER, true);

        REQUIRE(algorithm.initiate() == Results::PATH_NOT_FOUND);
    }

    SECTION ("WaveAlgorithm::initiate : 3D invalid input (out of bounds)") {
        // Координата X=3 выходит за границы матрицы 3x3x3 (допустимы 0, 1, 2)
        WaveAlgorithm algorithm = WaveAlgorithm(new Matrix3(3, 3, 3,
            '-', '-', '-',
            '-', '-', '-',
            '-', '-', '-',

            '-', '-', '-',
            '-', '-', '-',
            '-', '-', '-',

            '-', '-', '-',
            '-', '-', '-',
            '-', '-', '-'
        ), new Pos3(0, 0, 0), new Pos3(3, 0, 0), DEFAULT_ORDER, true);

        REQUIRE(algorithm.initiate() == Results::INVALID_INPUT);
    }

    SECTION ("WaveAlgorithm::initiate : 3D invalid input (collide with wall '#')") {
        // Точка 'B' установлена в ячейку с непроходимой стеной '#'
        WaveAlgorithm algorithm = WaveAlgorithm(new Matrix3(3, 3, 3,
            '-', '-', '-',
            '-', '-', '-',
            '-', '-', '-',

            '-', '-', '-',
            '-', '-', '-',
            '-', '-', '-',

            '-', '-', '-',
            '-', '-', '-',
            '-', '-', '#'
        ), new Pos3(0, 0, 0), new Pos3(2, 2, 2), DEFAULT_ORDER, true);

        REQUIRE(algorithm.initiate() == Results::INVALID_INPUT);
    }
}