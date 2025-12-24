#include "catch2/catch_test_macros.hpp"
#include "../src/memory/Pointer.hpp"

TEST_CASE("Pointer: default constructor", "[Pointer]") {
    Pointer p;

    REQUIRE((uint64_t) p == 0);
}

TEST_CASE("Pointer: size constructor", "[Pointer]") {
    Pointer p(10);

    REQUIRE((uint64_t) p == 0);
}

TEST_CASE("Pointer: increments", "[Pointer]") {
    Pointer p(10);
    
    SECTION("Pre-increment") {
        Pointer old = ++p;

        REQUIRE((uint64_t) old == 1);
        REQUIRE((uint64_t) p == 1);
    }
    
    SECTION("Post-increment") {
        Pointer old = p++;

        REQUIRE((uint64_t) old == 0);
        REQUIRE((uint64_t) p == 1);
    }
}

TEST_CASE("Pointer: decrements", "[Pointer]") {
    Pointer p(10);
    p = 5;
    
    SECTION("Pre-decrement") {
        Pointer old = --p;

        REQUIRE((uint64_t) old == 4);
        REQUIRE((uint64_t) p == 4);
    }
    
    SECTION("Post-decrement") {
        Pointer old = p--;

        REQUIRE((uint64_t) old == 5);
        REQUIRE((uint64_t) p == 4);
    }
}

TEST_CASE("Pointer: assignment", "[Pointer]") {
    Pointer p(10);
    
    SECTION("Assign within bounds") {
        p = 7;
        REQUIRE((uint64_t) p == 7);
    }

    SECTION("Assign beyond bounds") {
        p = 15;
        REQUIRE((uint64_t) p == 15 % 10);
        p = 22;
        REQUIRE((uint64_t) p == 22 % 10);
        p = 13;
        REQUIRE((uint64_t) p == 13 % 10);
    }

    SECTION("Add within bounds") {
        p = 2;
        p += 3;
        REQUIRE((uint64_t) p == 2 + 3);
    }

    SECTION("Add beyond bounds") {
        p = 7;
        p += 12;
        REQUIRE((uint64_t) p == (7 + 12) % 10);
    }

    SECTION("Subtract within bounds") {
        p = 5;
        p -= 3;
        REQUIRE((uint64_t) p == 5 - 3);
    }

    SECTION("Subtract beyond bounds") {
        p = 2;
        p -= 5;
        REQUIRE((uint64_t) p == 7);
    }
}

TEST_CASE("Pointer: wraparound", "[Pointer]") {
    Pointer p(10);
    
    SECTION("Wrap from end to start") {
        p = 9;
        ++p;
        REQUIRE((uint64_t) p == 0);
        --p;
        REQUIRE((uint64_t) p == 9);
    }
    
    SECTION("Wrap from start to end") {
        p = 0;
        p--;
        REQUIRE((uint64_t) p == 9);
        p++;
        REQUIRE((uint64_t) p == 0);
    }
}
