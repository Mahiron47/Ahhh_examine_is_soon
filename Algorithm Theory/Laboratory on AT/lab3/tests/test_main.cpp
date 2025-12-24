#define CATCH_CONFIG_MAIN
#include "../src/BrainfuckInterpretator.hpp"
#include "../src/logger/DebugCode.hpp"

#include "catch2/catch_test_macros.hpp"
#include <sstream>
#include <iostream>

TEST_CASE("BrainfuckInterpretator: increment", "[BrainfuckInterpretator]") {
	std::ostringstream oss;
	auto* old_buf = std::cout.rdbuf(oss.rdbuf()); // перенаправляем cout

    BrainfuckInterpretator interpreter;
    std::string code = "++++++++++++++++++++++++++++++++++++++++++++++++~";
    interpreter.init(code.c_str(), code.size());
    interpreter.exec();

    std::cout.rdbuf(old_buf); // восстанавливаем cout
    REQUIRE(oss.str() == "0");
}

TEST_CASE("BrainfuckInterpretator: decrement", "[BrainfuckInterpretator]") {
	std::ostringstream oss;
	auto* old_buf = std::cout.rdbuf(oss.rdbuf()); // перенаправляем cout

    BrainfuckInterpretator interpreter;
    std::string code = "++++++++++++++++++++++++++++++++++++++++++++++++------------------------------------------------++++++++++++++++++++++++++++++++++++++++++++++++~";
    interpreter.init(code.c_str(), code.size());
    interpreter.exec();

    std::cout.rdbuf(old_buf); // восстанавливаем cout
    REQUIRE(oss.str() == "0");
}

TEST_CASE("BrainfuckInterpretator: ini values", "[BrainfuckInterpretator]") {
	std::ostringstream oss;
	auto* old_buf = std::cout.rdbuf(oss.rdbuf()); // перенаправляем cout

    BrainfuckInterpretator interpreter;
    std::string code = "|------------->*++++++++>*++++++++>*|------------->*++++++++>|-------------~";
    interpreter.init(code.c_str(), code.size());
    interpreter.exec();

    std::cout.rdbuf(old_buf); // восстанавливаем cout
    REQUIRE(interpreter.get_mem_size() == 6);
    REQUIRE(oss.str() == "z!!z!z");
}

TEST_CASE("BrainfuckInterpretator: ini all values operator", "[BrainfuckInterpretator]") {
	std::ostringstream oss;
	auto* old_buf = std::cout.rdbuf(oss.rdbuf()); // перенаправляем cout

    BrainfuckInterpretator interpreter;
    std::string code = "|------------->>|------------->|------------->++++++++++++++++++++++++++++++++++++++++++++++++$~";
    interpreter.init(code.c_str(), code.size());
    interpreter.exec();

    std::cout.rdbuf(old_buf); // восстанавливаем cout
    REQUIRE(interpreter.get_mem_size() == 5);
    REQUIRE(oss.str() == "00000");
}

TEST_CASE("BrainfuckInterpretator: wrap around", "[BrainfuckInterpretator]") {
	std::ostringstream oss;
	auto* old_buf = std::cout.rdbuf(oss.rdbuf()); // перенаправляем cout

    BrainfuckInterpretator interpreter;
    std::string code = "|------------->*++++++++>*++++++++>*|------------->*++++++++><<<|-------------<<<<|------------->>>*++++++++>*++++++++>*++++++++>*++++++++>*++++++++>*++++++++>*++++++++>|-------------~";
    interpreter.init(code.c_str(), code.size());
    interpreter.exec();

    std::cout.rdbuf(old_buf); // восстанавливаем cout
    REQUIRE(interpreter.get_mem_size() == 6);
    REQUIRE(oss.str() == "!!z");
}

TEST_CASE("BrainfuckInterpretator: output operator", "[BrainfuckInterpretator]") {
	std::ostringstream oss;
	auto* old_buf = std::cout.rdbuf(oss.rdbuf()); // перенаправляем cout

    BrainfuckInterpretator interpreter;
    std::string code = "|------------->*++++++++>*++++++++>*|------------->*++++++++><<<|-------------<<<<|------------->>>*++++++++>*++++++++>*++++++++>*++++++++>*++++++++>*++++++++>*++++++++>|-------------<<~";
    interpreter.init(code.c_str(), code.size());
    interpreter.exec();

    std::cout.rdbuf(old_buf); // восстанавливаем cout
    REQUIRE(oss.str() == "!");
}

TEST_CASE("Assignment 1: output ASCII", "LAB") {
	std::ostringstream oss;
	auto* old_buf = std::cout.rdbuf(oss.rdbuf()); // перенаправляем cout

    BrainfuckInterpretator interpreter = BrainfuckInterpretator();
    
    std::vector<const char*> arg = {"-i", "C:\\Users\\Admin\\Desktop\\Ahhh_examine_is_soon\\Algorithm Theory\\Laboratory on AT\\lab3\\res\\ASCII.bf"};
    interpreter.init((int) arg.size(), const_cast<char**>(arg.data()));
    interpreter.exec();

    std::cout.rdbuf(old_buf); // восстанавливаем cout
    std::string expected;
    for (char c = 0; c != -128; c++) {
        expected += c;
    }
    REQUIRE(oss.str() == expected);
}

TEST_CASE("Assignment 2: output \"KAFEDRA PKIMS RULIT\"", "LAB") {
	std::ostringstream oss;
	auto* old_buf = std::cout.rdbuf(oss.rdbuf()); // перенаправляем cout

    BrainfuckInterpretator interpreter = BrainfuckInterpretator();

    std::vector<const char*> arg = {"-i", "C:\\Users\\Admin\\Desktop\\Ahhh_examine_is_soon\\Algorithm Theory\\Laboratory on AT\\lab3\\res\\KAFEDRA PKIMS RULIT.bf"};
    interpreter.init((int) arg.size(), const_cast<char**>(arg.data()));
    interpreter.exec();
    
    std::cout.rdbuf(old_buf); // восстанавливаем cout
   
    REQUIRE(oss.str() == "KAFEDRA PKIMS RULIT");
}

TEST_CASE("Assignment 3: output Assigment 2 ru ver", "LAB") {
	std::ostringstream oss;
	auto* old_buf = std::cout.rdbuf(oss.rdbuf()); // перенаправляем cout
    BrainfuckInterpretator interpreter = BrainfuckInterpretator();

    std::vector<const char*> arg = {"-i", "C:\\Users\\Admin\\Desktop\\Ahhh_examine_is_soon\\Algorithm Theory\\Laboratory on AT\\lab3\\res\\Task2ru.bf"};
    interpreter.init((int) arg.size(), const_cast<char**>(arg.data()));
    interpreter.exec();
    
    std::cout.rdbuf(old_buf); // восстанавливаем cout

    std::vector<unsigned char> expected_bytes = {
        0xCA, 0xC0, 0xD4, 0xC5, 0xC4, 0xD0, 0xC0, 0x20, 0xCF, 0xCA, 0xC8, 0xCC, 0xD1, 0x20, 0xD0, 0xD3, 0xCB, 0xC8, 0xD2
    };
    std::string expected(expected_bytes.begin(), expected_bytes.end());
}

TEST_CASE("ERROR: INPUT_FILE_UNDEFINED", "LAB") {
    BrainfuckInterpretator interpreter = BrainfuckInterpretator();

    std::vector<const char*> arg = {"-i"};
   
    REQUIRE_THROWS_AS(
        interpreter.init((int)arg.size(), const_cast<char**>(arg.data())),
        ErrorCode
    );
}

TEST_CASE("ERROR: INVALID_FILE_PATH", "LAB") {
    BrainfuckInterpretator interpreter = BrainfuckInterpretator();

    std::vector<const char*> arg = {"-i", "invalid_file_path.bf"};
   
    REQUIRE_THROWS_AS(
        interpreter.init((int)arg.size(), const_cast<char**>(arg.data())),
        ErrorCode
    );
}

TEST_CASE("ERROR: UNDEFINED_INSTRUCTION", "LAB") {
    BrainfuckInterpretator interpreter = BrainfuckInterpretator();

    std::string code = "++A++B++C$~";
    REQUIRE_THROWS_AS(
        interpreter.init(code.c_str(), code.size()),
        ErrorCode
    );
}