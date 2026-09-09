#include <iostream>
#include <concepts>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include <queue>
#include <span>

enum Conditions : uint8_t {
    CHECKED_BIT       = 0b0000'0001,
    POINT_A_BIT       = 0b0000'0010,
    POINT_B_BIT       = 0b0000'0100,
    MARK_AS_PATH_BIT  = 0b0000'1000,
    OUT_OF_BOUNDS_BIT = 0b1000'0000,
};

enum Directions : uint32_t {
    DIRECTION_FORWARD  = 0b000,
    DIRECTION_BACKWARD = 0b001,
    DIRECTION_RIGHT    = 0b010,
    DIRECTION_LEFT     = 0b011,
    DIRECTION_UP       = 0b100,
    DIRECTION_DOWN     = 0b101,
};

static constexpr uint32_t SEED = 521355634;
static double RANDOM_PREV_RESULT = 1.0;

inline static uint32_t get_random(uint32_t min, uint32_t max) noexcept {
    double t = std::sin(RANDOM_PREV_RESULT * std::sqrt(SEED) + SEED);
        
    RANDOM_PREV_RESULT = min + (t - std::abs(t)) * (max - min);

    return RANDOM_PREV_RESULT;
}

struct Ipos {
    virtual ~Ipos() noexcept = default;
};

struct Pos2 : Ipos {
    uint32_t x;
    uint32_t y;

    constexpr Pos2() noexcept = default;
    constexpr ~Pos2() noexcept = default;
    constexpr Pos2(uint32_t x, uint32_t y) noexcept : x(x), y(y) {}

    constexpr Pos2 operator+(Pos2 other) const noexcept {
        return Pos2( this->x + other.x,
                     this->y + other.y );
    }
};

struct Pos3 : Ipos {
    uint32_t x;
    uint32_t y;
    uint32_t z;

    constexpr Pos3() noexcept = default;
    constexpr ~Pos3() noexcept = default;
    constexpr Pos3(uint32_t x, uint32_t y, uint32_t z) noexcept : x(x), y(y), z(z) {}

    constexpr Pos3 operator+(Pos3 other) const noexcept {
        return Pos3( this->x + other.x,
                     this->y + other.y,
                     this->z + other.z );
    }
};

inline constexpr uint8_t operator|(uint8_t lhs, Conditions rhs) noexcept {
    return static_cast<uint8_t>(lhs | static_cast<uint8_t>(rhs));
}

#include "matrix/Imatrix.hpp"
#include "matrix/Matrix2.hpp"
#include "matrix/Matrix3.hpp"
#include "algorithm/WaveAlgorithm.hpp"
