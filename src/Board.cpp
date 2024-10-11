//
// Created by Nihar Gagneja on 9/28/24.
//

#include "Board.h"

#include <iostream>
#include <ostream>
#include <random>

Vector2i Board::getTileDimensions() const {
    return Vector2i {m_screenDimensions.x / m_numTiles.x, m_screenDimensions.y / m_numTiles.y};
}

void Board::initialize() {
    // set up random number generator
    std::srand(std::time(nullptr));

    // set up snake
    m_snake.reset();

    placeFoodRandomly();
}

void Board::placeFoodRandomly() {
    Vector2i foodPosition = {0, 0};

    // find a random food position that isn't occupied by the snake already
    do {
        foodPosition.x = std::rand() % getDimensions().x;
        foodPosition.y = std::rand() % getDimensions().y;
    } while (m_snake.hasSegmentAt(foodPosition));


    m_food.setPosition(foodPosition);
    std::cout << "Food position: " << foodPosition.toString() << std::endl;
}

void Board::processPlayerInput(const Direction direction) {
    Vector2i directionAsVector = {0, 0};

    switch (direction) {
        case Direction::UP:
            m_snake.changeDirection(DIRECTION_UP);
        break;
        case Direction::DOWN:
            m_snake.changeDirection(DIRECTION_DOWN);
        break;
        case Direction::LEFT:
            m_snake.changeDirection(DIRECTION_LEFT);
        break;
        case Direction::RIGHT:
            m_snake.changeDirection(DIRECTION_RIGHT);
        break;
        default:
            throw std::invalid_argument("Invalid direction");
    }
}

void Board::update() {
    m_snake.update();

    if (m_snake.headIsAt(m_food.getPosition())) {
        m_snake.grow();
        placeFoodRandomly();
    }
}

void Board::draw() const {
    drawTiles();
    m_snake.draw();
    m_food.draw(getTileDimensions());
}

void Board::drawTiles() const {
    for (int x = 0; x < m_screenDimensions.x; x++) {
        for (int y = 0; y < m_screenDimensions.y; y++) {
            DrawRectangleLines(x * getTileDimensions().x, y * getTileDimensions().y, getTileDimensions().x, getTileDimensions().y, WHITE);
        }
    }
}

