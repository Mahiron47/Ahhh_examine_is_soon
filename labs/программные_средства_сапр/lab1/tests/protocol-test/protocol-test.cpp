#include <catch2/catch_all.hpp>
#include <sstream>
#include <string>
#include <iostream>

#include "DynamicLib.hpp"

namespace {

template<typename Func>
std::string capture_stdout(Func&& func) {
    std::stringstream buffer;
    std::streambuf* old_cout = std::cout.rdbuf(buffer.rdbuf());
    func();
    std::cout.rdbuf(old_cout);
    std::string raw = buffer.str();
    
    // Normalize \r\n to \n for consistent cross-platform string comparison
    std::string normalized;
    normalized.reserve(raw.size());
    for (size_t i = 0; i < raw.size(); ++i) {
        if (raw[i] == '\r' && i + 1 < raw.size() && raw[i + 1] == '\n') {
            continue;
        }
        normalized.push_back(raw[i]);
    }
    return normalized;
}

// Printer from task3 (wave propagation)
[[maybe_unused]] auto make_wave_printer(uint32_t max_sector_length) {
    return [max_sector_length](Imatrix::Element elem) -> std::string {
        if (elem.condition & Conditions::POINT_A_BIT) return "A" + std::string(max_sector_length, ' ');
        if (elem.condition & Conditions::POINT_B_BIT) return "B" + std::string(max_sector_length, ' ');
        if (elem.info != 0) return std::to_string(elem.info) + std::string(max_sector_length - std::to_string(elem.info).length() + 1, ' ');

        return std::string(1, elem.symbol) + std::string(max_sector_length, ' ');
    };
}

// Printer from solution-maximum (shortest path '#')
[[maybe_unused]] auto path_printer = [](Imatrix::Element elem) -> std::string {
    if (elem.condition & Conditions::POINT_A_BIT) return "A";
    if (elem.condition & Conditions::POINT_B_BIT) return "B";
    if (elem.condition & Conditions::MARK_AS_PATH_BIT) return "#";

    return std::string(1, elem.symbol);
};

// Raw / initial grid printer
[[maybe_unused]] auto initial_printer = [](Imatrix::Element elem) -> std::string {
    if (elem.condition & Conditions::POINT_A_BIT) return "A";
    if (elem.condition & Conditions::POINT_B_BIT) return "B";

    return std::string(1, elem.symbol);
};

} // anonymous namespace

TEST_CASE("ProtocolTest::CaptureInitialMatrix", "[protocol]") {
    Matrix2 matrix(3, 3,
        '-', '-', '%',
        '-', '%', '-',
        '-', '-', '-'
    );

    std::string captured = capture_stdout([&]() {
        matrix.print(initial_printer);
    });

    std::string expected =
        "- - %\n"
        "- % -\n"
        "- - -\n";

    REQUIRE(captured == expected);
}

TEST_CASE("ProtocolTest::CorridorPathSolutionMaximum", "[protocol]") {
    Matrix2 matrix(5, 5,
        '-', '-', '%', '-', '-',
        '%', '-', '%', '-', '%',
        '%', '-', '-', '-', '%',
        '%', '%', '%', '-', '%',
        '-', '-', '-', '-', '-'
    );

    WaveAlgorithm algorithm(&matrix, new Pos2(0, 0), new Pos2(4, 4), DEFAULT_ORDER);
    uint32_t result = algorithm.initiate();
    REQUIRE(result == Results::SUCCESS);
    REQUIRE(algorithm.get_path_length() == 8);

    std::string captured = capture_stdout([&]() {
        matrix.print(path_printer);
    });

    std::string expected =
        "A # % - -\n"
        "% # % - %\n"
        "% # # # %\n"
        "% % % # %\n"
        "- - - # B\n";

    REQUIRE(captured == expected);
}

TEST_CASE("ProtocolTest::CorridorWavePropagationTask3", "[protocol]") {
    Matrix2 matrix(5, 5,
        '-', '-', '%', '-', '-',
        '%', '-', '%', '-', '%',
        '%', '-', '-', '-', '%',
        '%', '%', '%', '-', '%',
        '-', '-', '-', '-', '-'
    );

    WaveAlgorithm algorithm(&matrix, new Pos2(0, 0), new Pos2(4, 4), DEFAULT_ORDER);
    uint32_t result = algorithm.initiate();
    REQUIRE(result == Results::SUCCESS);

    uint32_t max_sector_length = std::to_string(algorithm.get_path_length()).length() - 1;
    std::string captured = capture_stdout([&]() {
        matrix.print(make_wave_printer(max_sector_length));
    });

    // Verify that wave numbers expand from A(0,0) to B(4,4)
    // and unreachable or unreached cells retain '-' or '%'
    std::string expected =
        "A 1 % 7 8\n"
        "% 2 % 6 %\n"
        "% 3 4 5 %\n"
        "% % % 6 %\n"
        "- - 8 7 B\n";

    REQUIRE(captured == expected);
}

TEST_CASE("ProtocolTest::SnakePathSolutionMaximum", "[protocol]") {
    // 6x6 matrix with a snake corridor forcing a 19-step path
    Matrix2 matrix(6, 6,
        '-', '-', '-', '-', '-', '-',
        '%', '%', '%', '%', '%', '-',
        '-', '-', '-', '-', '-', '-',
        '-', '%', '%', '%', '%', '%',
        '-', '-', '-', '-', '-', '-',
        '-', '-', '-', '-', '-', '-'
    );

    WaveAlgorithm algorithm(&matrix, new Pos2(0, 0), new Pos2(5, 4), DEFAULT_ORDER);
    uint32_t result = algorithm.initiate();
    REQUIRE(result == Results::SUCCESS);
    REQUIRE(algorithm.get_path_length() == 19);

    std::string captured_path = capture_stdout([&]() {
        matrix.print(path_printer);
    });

    std::string expected_path =
        "A # # # # #\n"
        "% % % % % #\n"
        "# # # # # #\n"
        "# % % % % %\n"
        "# # # # # B\n"
        "- - - - - -\n";

    REQUIRE(captured_path == expected_path);
}

TEST_CASE("ProtocolTest::SnakeWavePropagationTask3Aligned", "[protocol]") {
    // 6x6 matrix testing 2-digit wave number alignment (max_sector_length == 1)
    Matrix2 matrix(6, 6,
        '-', '-', '-', '-', '-', '-',
        '%', '%', '%', '%', '%', '-',
        '-', '-', '-', '-', '-', '-',
        '-', '%', '%', '%', '%', '%',
        '-', '-', '-', '-', '-', '-',
        '-', '-', '-', '-', '-', '-'
    );

    WaveAlgorithm algorithm(&matrix, new Pos2(0, 0), new Pos2(5, 4), DEFAULT_ORDER);
    uint32_t result = algorithm.initiate();
    REQUIRE(result == Results::SUCCESS);
    REQUIRE(algorithm.get_path_length() == 19);

    uint32_t max_sector_length = std::to_string(algorithm.get_path_length()).length() - 1;
    REQUIRE(max_sector_length == 1);

    std::string captured_wave = capture_stdout([&]() {
        matrix.print(make_wave_printer(max_sector_length));
    });

    // Each column is 2 characters wide, separated by space
    std::string expected_wave =
        "A  1  2  3  4  5 \n"
        "%  %  %  %  %  6 \n"
        "12 11 10 9  8  7 \n"
        "13 %  %  %  %  % \n"
        "14 15 16 17 18 B \n"
        "15 16 17 18 19 20\n";

    REQUIRE(captured_wave == expected_wave);
}

TEST_CASE("ProtocolTest::UnreachableTargetNoPath", "[protocol]") {
    // Column 2 is a solid wall dividing point A from point B
    Matrix2 matrix(4, 4,
        '-', '-', '%', '-',
        '-', '-', '%', '-',
        '-', '-', '%', '-',
        '-', '-', '%', '-'
    );

    WaveAlgorithm algorithm(&matrix, new Pos2(1, 1), new Pos2(3, 1), DEFAULT_ORDER);
    uint32_t result = algorithm.initiate();
    REQUIRE(result == Results::PATH_NOT_FOUND);

    std::string captured_path = capture_stdout([&]() {
        matrix.print(path_printer);
    });

    // When path is not found, no cells are marked with '#'; only A, B, %, - appear
    std::string expected_path =
        "- - % -\n"
        "- A % B\n"
        "- - % -\n"
        "- - % -\n";

    REQUIRE(captured_path == expected_path);
}





