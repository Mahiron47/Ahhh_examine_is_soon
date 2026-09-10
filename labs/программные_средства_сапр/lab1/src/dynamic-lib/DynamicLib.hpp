#ifndef DYNAMICLIBHPP
#define DYNAMICLIBHPP

#include <iostream>
#include <concepts>
#include <stdint.h>
#include <assert.h>
#include <math.h>
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

enum Results : uint32_t {
    SUCCESS        = 0b1,
    PATH_NOT_FOUND = 0b10,
    INVALID_INPUT  = 0b100,
};

#define DEFAULT_ORDER Directions::DIRECTION_FORWARD | (Directions::DIRECTION_BACKWARD << 3) | \
        (Directions::DIRECTION_RIGHT << 6) | (Directions::DIRECTION_LEFT << 9) | \
        (Directions::DIRECTION_UP << 12) | (Directions::DIRECTION_DOWN << 15)

static constexpr uint32_t SEED = 52634;
static double RANDOM_PREV_RESULT = 11.4325;

inline static uint32_t get_random(uint32_t min, uint32_t max) noexcept {
    double t = std::sin(RANDOM_PREV_RESULT * SEED) * SEED;
        
    RANDOM_PREV_RESULT = min + std::abs(t - std::floor(t)) * (max - min);

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

    constexpr bool operator==(Pos2 other) const noexcept {
        return other.x == this->x && other.y == this->y;
    }

    constexpr bool operator!=(Pos2 other) const noexcept {
        return !(other == *this);
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

    constexpr bool operator==(Pos3 other) const noexcept {
        return other.x == this->x && other.y == this->y && other.z == this->z;
    }

    constexpr bool operator!=(Pos3 other) const noexcept {
        return !(other == *this);
    }
};

inline constexpr uint8_t operator|(uint8_t lhs, Conditions rhs) noexcept {
    return static_cast<uint8_t>(lhs | static_cast<uint8_t>(rhs));
}

inline constexpr uint8_t operator&(uint8_t lhs, Conditions rhs) noexcept {
    return static_cast<uint8_t>(lhs & static_cast<uint8_t>(rhs));
}

inline constexpr uint8_t operator|(Conditions lhs, Conditions rhs) noexcept {
    return static_cast<uint8_t>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline constexpr uint8_t operator&(Conditions lhs, Conditions rhs) noexcept {
    return static_cast<uint8_t>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

inline constexpr uint8_t operator==(Conditions lhs, Conditions rhs) noexcept {
    return static_cast<uint8_t>(static_cast<uint8_t>(lhs) == static_cast<uint8_t>(rhs));
}

inline constexpr uint8_t operator!=(Conditions lhs, Conditions rhs) noexcept {
    return static_cast<uint8_t>(static_cast<uint8_t>(lhs) != static_cast<uint8_t>(rhs));
}

#include "matrix/Imatrix.hpp"
#include "matrix/Matrix2.hpp"
#include "matrix/Matrix3.hpp"
#include "algorithm/WaveAlgorithm.hpp"

#endif // DYNAMICLIBHPP