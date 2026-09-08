#include <iostream>
#include <concepts>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include <queue>
#include <span>

#define ORDER (DIRECTION_UP << 15) | (DIRECTION_DOWN << 12) | (DIRECTION_RIGHT << 9) | (DIRECTION_LEFT << 6) | (DIRECTION_FORWARD << 3) | DIRECTION_BACKWARD
#define UNCHECKED 0
#define CHECKED 1

#include "matrix/Imatrix.hpp"
#include "matrix/Matrix2.hpp"
#include "matrix/Matrix3.hpp"
#include "container/ChainedList.hpp"
#include "algorithm/WaveAlgorithm.hpp"

static constexpr uint32_t SEED = 521355634;
static double RANDOM_PREV_RESULT = 1.0;

inline static uint32_t get_random(uint32_t min, uint32_t max) noexcept {
    double t = std::sin(RANDOM_PREV_RESULT * std::sqrt(SEED) + SEED);
        
    RANDOM_PREV_RESULT = min + (t - std::abs(t)) * (max - min);

    return RANDOM_PREV_RESULT;
}

enum Directions : uint32_t {
    DIRECTION_FORWARD  = 0b000,
    DIRECTION_BACKWARD = 0b001,
    DIRECTION_RIGHT    = 0b010,
    DIRECTION_LEFT     = 0b011,
    DIRECTION_UP       = 0b100,
    DIRECTION_DOWN     = 0b101,

};

struct Ipos {
    virtual ~Ipos() noexcept = default;
};

struct Pos2 : Ipos {
    uint32_t x;
    uint32_t y;

    Pos2() = default;
    Pos2(uint32_t x, uint32_t y) noexcept : x(x), y(y) {}

    constexpr Pos2 operator+(Pos2 other) const noexcept {
        return Pos2(
            this->x + other.x,
            this->y + other.y
        );
    }
};

struct Pos3 : Ipos {
    uint32_t x;
    uint32_t y;
    uint32_t z;

    Pos3() = default;
    Pos3(uint32_t x, uint32_t y, uint32_t z) noexcept : x(x), y(y), z(z) {}

    constexpr Pos3 operator+(Pos3 other) const noexcept {
        return Pos3(
            this->x + other.x,
            this->y + other.y,
            this->z + other.z
        );
    }
};
