//
// Created by Nihar Gagneja on 9/29/24.
//

#ifndef SNAKESEGMENT_H
#define SNAKESEGMENT_H

#include "raylib.h"
#include "Vector2i.h"

// // not used as a member of SnakeSegment; calculated in realtime when drawing graphics
enum class SnakeSegmentOrientation {
    STRAIGHT_VERTICAL,
    STRAIGHT_HORIZONTAL,

    BENT_UP_TO_RIGHT,
    BENT_UP_TO_LEFT,
    BENT_DOWN_TO_RIGHT,
    BENT_DOWN_TO_LEFT
};

class SnakeSegment {
private:
    Vector2i m_position;
    Color m_color;

public:
    // Constructor
    SnakeSegment(const Vector2i& position, const Color& color) : m_position(position), m_color(color) {};

    // Getters
    [[nodiscard]] Vector2i getPosition() const { return m_position; }
    [[nodiscard]] Color getColor() const { return m_color; }

    // Setters
    void setPosition(const Vector2i &position) { m_position = position; }
};



#endif //SNAKESEGMENT_H
