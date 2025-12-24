#include "catch2/catch_test_macros.hpp"
#include "../src/memory/Memory.hpp"
#include <sstream>

TEST_CASE("Memory: full constructor", "[Memory]") {
    const char* data = "hello";
    Memory mem(data, 5);
    
    REQUIRE(mem[0] == 'h');
    REQUIRE(mem[1] == 'e');
    REQUIRE(mem[2] == 'l');
    REQUIRE(mem[3] == 'l');
    REQUIRE(mem[4] == 'o');
}

TEST_CASE("Memory: only size constructor", "[Memory]") {
    Memory mem(10);
    
    for (uint64_t i = 0; i < 10; i++) {
        REQUIRE(mem[i] == 0);
    }
}

TEST_CASE("Memory: read operations", "[Memory]") {
    const char* data = "test";
    Memory mem(data, 4);

    REQUIRE(mem[0] == 't');
    REQUIRE(mem[1] == 'e');
    REQUIRE(mem[2] == 's');
    REQUIRE(mem[3] == 't');
}

TEST_CASE("Memory: write operations", "[Memory]") {
    Memory mem(10);
    
    mem[0] = 'T';
    REQUIRE(mem[0] == 'T');
    mem[1] = 'e';
    mem[2] = 's';
    REQUIRE(mem[1] == 'e'); 
    REQUIRE(mem[2] == 's');
    mem[3] = 't';
    REQUIRE(mem[3] == 't');
}

TEST_CASE("Memory: copy assignment", "[Memory]") {
    Memory mem1("abcde", 5);
    Memory mem2("12345", 5);
    
    mem2 = mem1;
    
    REQUIRE(mem2[0] == 'a');
    REQUIRE(mem2[1] == 'b');
    REQUIRE(mem2[2] == 'c');
    REQUIRE(mem2[3] == 'd');
    REQUIRE(mem2[4] == 'e');
}

TEST_CASE("Memory: char assignment", "[Memory]") {
    Memory mem("hello", 5);
    
    mem = (uint64_t) 0;  
    mem = (char) 135;    
    REQUIRE(mem[0] == (char)135);
}

TEST_CASE("Memory: increments", "[Memory]") {
    Memory mem("hello", 5);
    
    SECTION("Pre-increment") {
        Memory old = ++mem;
        REQUIRE(mem[0] == 'h');
        REQUIRE(mem[1] == 'e');
        REQUIRE(mem[2] == 'l');
        REQUIRE(mem[3] == 'l');
        REQUIRE(mem[4] == 'o');
        mem = 'A';
        REQUIRE(mem[0] == 'h');
        REQUIRE(mem[1] == 'A');
        REQUIRE(mem[2] == 'l');
        REQUIRE(mem[3] == 'l');
        REQUIRE(mem[4] == 'o');
        old = 'B';
        REQUIRE(old[0] == 'h');
        REQUIRE(old[1] == 'B');
        REQUIRE(old[2] == 'l');
        REQUIRE(old[3] == 'l');
        REQUIRE(old[4] == 'o');
    }

    SECTION("Post-increment") {
        Memory old = mem++;
        REQUIRE(mem[0] == 'h');
        REQUIRE(mem[1] == 'e');
        REQUIRE(mem[2] == 'l');
        REQUIRE(mem[3] == 'l');
        REQUIRE(mem[4] == 'o');
        mem = 'C';
        REQUIRE(mem[0] == 'h');
        REQUIRE(mem[1] == 'C');
        REQUIRE(mem[2] == 'l');
        REQUIRE(mem[3] == 'l');
        REQUIRE(mem[4] == 'o');
        old = 'D';
        REQUIRE(old[0] == 'D');
        REQUIRE(old[1] == 'e');
        REQUIRE(old[2] == 'l');
        REQUIRE(old[3] == 'l');
        REQUIRE(old[4] == 'o');
    }
}

TEST_CASE("Memory: decrements", "[Memory]") {
    Memory mem("world", 5);
    mem = (uint64_t) 4;  
    
    SECTION("Pre-decrement") {
        Memory old = --mem;
        REQUIRE(mem[0] == 'w');
        REQUIRE(mem[1] == 'o');
        REQUIRE(mem[2] == 'r');
        REQUIRE(mem[3] == 'l');
        REQUIRE(mem[4] == 'd');
        mem = 'A';
        REQUIRE(mem[0] == 'w');
        REQUIRE(mem[1] == 'o');
        REQUIRE(mem[2] == 'r');
        REQUIRE(mem[3] == 'A');
        REQUIRE(mem[4] == 'd');
        old = 'B';
        REQUIRE(old[0] == 'w');
        REQUIRE(old[1] == 'o');
        REQUIRE(old[2] == 'r');
        REQUIRE(old[3] == 'B');
        REQUIRE(old[4] == 'd');
    }
    
    SECTION("Post-decrement") {
        Memory old = mem--;
        REQUIRE(mem[0] == 'w');
        REQUIRE(mem[1] == 'o');
        REQUIRE(mem[2] == 'r');
        REQUIRE(mem[3] == 'l');
        REQUIRE(mem[4] == 'd');
        mem = 'C';
        REQUIRE(mem[0] == 'w');
        REQUIRE(mem[1] == 'o');
        REQUIRE(mem[2] == 'r');
        REQUIRE(mem[3] == 'C');
        REQUIRE(mem[4] == 'd');
        old = 'D';
        REQUIRE(old[0] == 'w');
        REQUIRE(old[1] == 'o');
        REQUIRE(old[2] == 'r');
        REQUIRE(old[3] == 'l');
        REQUIRE(old[4] == 'D');
    }
}

TEST_CASE("Memory: uint64_t assignment", "[Memory]") {
    Memory mem("12345", 5);
    
    SECTION("Assign within bounds") {
        mem = (uint64_t) 3;
        REQUIRE((uint64_t) mem == 3);
    }

    SECTION("Assign beyond bounds") {
        mem = (uint64_t) 9;
        REQUIRE((uint64_t) mem == 4);
        mem = (uint64_t) 10;
        REQUIRE((uint64_t) mem == 0);
        mem = (uint64_t) 11;
        REQUIRE((uint64_t) mem == 1);
    }

    SECTION("Add within bounds") {
        mem = (uint64_t) 2;
        mem += (uint64_t) 2;
        REQUIRE((uint64_t) mem == 2 + 2);
    }

    SECTION("Add beyond bounds") {
        mem = (uint64_t) 4;
        mem += (uint64_t) 3;
        REQUIRE((uint64_t) mem == (4 + 3) % 5);
    }

    SECTION("Subtract within bounds") {
        mem = (uint64_t) 4;
        mem -= (uint64_t) 2;
        REQUIRE((uint64_t) mem == 4 - 2);
    }  

    SECTION("Subtract beyond bounds") {
        mem = (uint64_t) 1;
        mem -= (uint64_t) 3;
        REQUIRE((uint64_t) mem == ((uint64_t) 1 - 3 - 1) % 5);
    }
}

TEST_CASE("Memory: stream output", "[Memory]") {
    Memory mem("test", 4);
    mem = (uint64_t) 3;
    std::ostringstream oss;
    
    oss << mem;
    REQUIRE(oss.str() == "test");
}

TEST_CASE("Memory: multiple modifications", "[Memory]") {
    Memory mem(1);
    
    mem = (char) 135; 
    mem += (char) 1; 
    mem += (char) 1; 
    mem += (char) 1; 
    mem += (char) 1; 
    mem += (char) 1; 
    REQUIRE(mem[0] == (char)(135 + 5));
    mem -= (char)1; 
    mem -= (char)1; 
    mem -= (char)1; 
    mem -= (char)1; 
    mem -= (char)1; 
    REQUIRE(mem[0] == (char)(135));
}

TEST_CASE("Memory: output", "[Memory]") {
    Memory mem(1);
    
    mem = (char) 65; 
    
    std::ostringstream oss;
    oss << mem;
    REQUIRE(oss.str() == std::string(1, (char)(65)));
}

TEST_CASE("Memory: modification and output", "[Memory]") {
    Memory mem(1);
    
    mem = (char) 65; 
    mem += (char) 1; 
    mem += (char) 1; 
    mem += (char) 1; 
    mem += (char) 1; 
    mem += (char) 1; 
    
    std::ostringstream oss;
    oss << mem;
    REQUIRE(oss.str() == std::string(1, (char)(65 + 5)));
}