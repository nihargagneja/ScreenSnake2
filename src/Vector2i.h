//
// Created by Nihar Gagneja on 9/28/24.
//


#ifndef VECTOR2I_H
#define VECTOR2I_H

#include <cmath>
#include <string>

struct Vector2i {
    int x;
    int y;

    // allows checks such as:
    // Vector2i direction = {0, -1}
    // if(direction == DIRECTION_UP)
    bool operator==(const Vector2i & other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2i & other) const {
        return !(*this == other);
    }

    // allows ability to "add" the direction to the position for movement
    Vector2i operator+(const Vector2i & other) const {
        return Vector2i {x + other.x, y + other.y};
    }

    Vector2i operator+=(const Vector2i & other) {
        return Vector2i{x += other.x,
        y += other.y};
    }

    Vector2i operator%(const Vector2i & other) const {
        return Vector2i{x % other.x, y % other.y};
    }

    explicit operator bool() const {
        return (x != 0 || y != 0);
    }

    Vector2i operator*(const int & scalar) const {
        return Vector2i{x * scalar, y * scalar};
    }

    [[nodiscard]] Vector2i wrapped(const Vector2i lowerBounds, const Vector2i upperBounds) const {
        return Vector2i {(this->x + upperBounds.x) % upperBounds.x, (this->y + upperBounds.y) % upperBounds.y};
    }

    Vector2i operator-(const Vector2i & other) const {
        return Vector2i {x - other.x, y - other.y};
    }

    bool oppositeTo(const Vector2i & other) const {
        return  (this->x == -other.x && this->y ==  other.y) ||
                (this->x ==  other.x && this->y == -other.y);
    }

    [[nodiscard]] std::string toString() const { return std::to_string(x) + ", " + std::to_string(y); }

    static Vector2i abs(Vector2i vec) {
        return Vector2i {std::abs(vec.x), std::abs(vec.y)};
    }
};

const Vector2i DIRECTION_UP = {0, -1};
const Vector2i DIRECTION_DOWN = {0, 1};
const Vector2i DIRECTION_LEFT = {-1, 0};
const Vector2i DIRECTION_RIGHT = {1, 0};

#endif //VECTOR2I_H
