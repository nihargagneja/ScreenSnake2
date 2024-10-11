//
// Created by Nihar Gagneja on 10/7/24.
//

#ifndef BOARD_H
#define BOARD_H
#include "Snake.h"
#include "Vector2i.h"
#include "Direction.h"
#include "Food.h"


class Board {
private:
    const Vector2i m_screenDimensions;
    const Vector2i m_numTiles;

    Snake m_snake;
    Food m_food;

public:
    Board(const Vector2i dimensions, const Vector2i numTiles) : m_screenDimensions(dimensions), m_numTiles(numTiles), m_snake(Snake(*this)) {};

    [[nodiscard]] Vector2i getTileDimensions() const;

    void initialize();

    void placeFoodRandomly();

    void processPlayerInput(Direction direction);
    void update();
    void draw() const;

    [[nodiscard]] Vector2i getDimensions() const { return {20, 20}; }
    void drawTiles() const;
};



#endif //BOARD_H
