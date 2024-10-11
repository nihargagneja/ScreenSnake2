//
// Created by Nihar Gagneja on 9/29/24.
//

#ifndef SNAKE_H
#define SNAKE_H
#include <vector>
#include <raylib.h>

#include "Food.h"
#include "SnakeSegment.h"

class Board;

class Snake {
private:
    // Snake data
    std::vector<std::unique_ptr<SnakeSegment>> m_segments;
    Vector2i m_headDirection;
    const Board& m_board; // read-only access to board data like position of food, tile sizes for drawing, etc.

    // Graphics assets
    const Texture2D snakeHeadTexture            =  LoadTexture("../assets/img/Snake01_Head.png");
    const Texture2D snakeStraightSegmentTexture =  LoadTexture("../assets/img/Snake01_Body.png");
    const Texture2D snakeBentSegmentTexture     =  LoadTexture("../assets/img/Snake01_Bend.png");
    const Texture2D snakeTailTexture            =  LoadTexture("../assets/img/Snake01_Tail.png");
    const Texture2D snakeDerpTexture            =  LoadTexture("../assets/img/Snake01_Derp.png");

    // internal functions for update and draw
    void move();
    static void drawSegment(const Texture2D &texture, Vector2 position, float rotation);
    [[nodiscard]] SnakeSegmentOrientation getOrientationOfSegment(int i) const;
    [[nodiscard]] bool isOnLeftOrRightEdge(const Vector2i& pos) const;
    [[nodiscard]] bool isOnTopOrBottomEdge(const Vector2i& pos) const;


public:
    explicit Snake(const Board& board) : m_headDirection(DIRECTION_RIGHT), m_board(board) {}

    // core of game
    void reset(); // also used as init
    void update();
    void grow();
    void draw() const;


    void changeDirection(Vector2i newDirection);

    // Information queries
    [[nodiscard]] bool selfCollisionOccurred() const;
    [[nodiscard]] bool hasSegmentAt(Vector2i position) const;

    [[nodiscard]] bool headIsAt(const Vector2i position) const {
        return m_segments[0]->getPosition() == position;
    };
};


#endif //SNAKE_H

