//
// Created by Nihar Gagneja on 10/7/24.
//

#include <iostream>

#include "Snake.h"
#include "Board.h"

/// GAME LOOP FUNCTIONS ///
void Snake::update() {
    move();
    if(selfCollisionOccurred()) {
        reset();
    }
}

void Snake::move() {
    if(m_segments.empty()) throw std::invalid_argument("Snake::move() called with empty m_segments");

    // *OTHER THAN THE HEAD*, go BACKWARDS through the snake segment, and set the position
    // of each segment to the position of the segment before it
    for(std::size_t i = m_segments.size() - 1; i >= 1; i--) {
        Vector2i positionOfPrecedingSegment = m_segments[i - 1]->getPosition();
        m_segments[i]->setPosition(positionOfPrecedingSegment);
    }

    // Move snake head, and handle wrapping
    const Vector2i newHeadPosition = (m_segments[0]->getPosition() + m_headDirection)
    .wrapped(
        {0, 0},
        m_board.getDimensions()
    );
    m_segments[0]->setPosition(newHeadPosition);



}


void Snake::grow() {
    // Special case: Snake is just a head
    if (m_segments.size() == 1) {
        Vector2i newSegmentPosition = m_segments[0]->getPosition() - m_headDirection;
        m_segments.push_back(std::make_unique<SnakeSegment>(
                                    newSegmentPosition,
                                    GOLD
                            )
        );
    }


    else {
        // calculate where to place the next segment based on relationship of last two segments
        Vector2i lastSegmentPosition = m_segments.back()->getPosition();
        Vector2i secondToLastSegmentPosition = m_segments[m_segments.size() - 2]->getPosition();
        Vector2i difference = lastSegmentPosition - secondToLastSegmentPosition;

        Vector2i newSegmentPosition = (lastSegmentPosition + difference).wrapped(
            {0, 0},
            m_board.getDimensions()
        );

        m_segments.push_back(std::make_unique<SnakeSegment>(newSegmentPosition, GOLD));
    }
}

SnakeSegmentOrientation Snake::getOrientationOfSegment(const int i) const {
    if (i >= m_segments.size()) throw std::out_of_range("Snake::getOrientationOfSegment called with index out of range.");

    // Get the current, previous, and next segment positions
    auto prevSegmentPos    = m_segments[i - 1]->getPosition();
    auto currentSegmentPos = m_segments[i + 0]->getPosition();
    auto nextSegmentPos    = m_segments[i + 1]->getPosition();

    // translate positions of wrapping segments to the center of the screen and do the math there
    const bool anyOnLeftOrRightEdge = isOnLeftOrRightEdge(prevSegmentPos) || isOnLeftOrRightEdge(currentSegmentPos) || isOnLeftOrRightEdge(nextSegmentPos);
    const bool anyOnTopOrBottomEdge = isOnTopOrBottomEdge(prevSegmentPos) || isOnTopOrBottomEdge(currentSegmentPos) || isOnTopOrBottomEdge(nextSegmentPos);

    if (anyOnLeftOrRightEdge || anyOnTopOrBottomEdge) {
        // decimal truncation will occur (e.g. 2.5 -> 2) if board dimensions are odd, but
        // application is equivalent to all segments and therefore not relevant
        const Vector2i TRANSLATE = {
            anyOnLeftOrRightEdge ? m_board.getDimensions().x / 2 : 0,
            anyOnTopOrBottomEdge ? m_board.getDimensions().y / 2 : 0
        };

        prevSegmentPos = (prevSegmentPos + TRANSLATE).wrapped(
            {0, 0},
            m_board.getDimensions()
        );

        currentSegmentPos = (currentSegmentPos + TRANSLATE).wrapped(
            {0, 0},
            m_board.getDimensions()
        );

        nextSegmentPos = (nextSegmentPos + TRANSLATE).wrapped(
            {0, 0},
            m_board.getDimensions()
        );
    }

    const auto diffNext = nextSegmentPos - currentSegmentPos;
    const auto diffPrev = currentSegmentPos - prevSegmentPos;

    // Check if the segment is straight (horizontal or vertical)
    if ((diffPrev == DIRECTION_LEFT && diffNext == DIRECTION_LEFT) || (diffPrev == DIRECTION_RIGHT && diffNext == DIRECTION_RIGHT)) {
        return SnakeSegmentOrientation::STRAIGHT_HORIZONTAL;
    }

    if ((diffPrev == DIRECTION_UP && diffNext == DIRECTION_UP) || (diffPrev == DIRECTION_DOWN && diffNext == DIRECTION_DOWN)) {
        return SnakeSegmentOrientation::STRAIGHT_VERTICAL;
    }

    // Check if the segment is bent by looking at the relative positions
    if ((diffNext == DIRECTION_DOWN && diffPrev == DIRECTION_LEFT) || (diffNext == DIRECTION_RIGHT && diffPrev == DIRECTION_UP)) {
        return SnakeSegmentOrientation::BENT_DOWN_TO_RIGHT;
    }

    if ((diffNext == DIRECTION_UP && diffPrev == DIRECTION_LEFT) || (diffNext == DIRECTION_RIGHT && diffPrev == DIRECTION_DOWN)) {
        return SnakeSegmentOrientation::BENT_UP_TO_RIGHT;
    }

    if ((diffNext == DIRECTION_DOWN && diffPrev == DIRECTION_RIGHT) || (diffNext == DIRECTION_LEFT && diffPrev == DIRECTION_UP)) {
        return SnakeSegmentOrientation::BENT_DOWN_TO_LEFT;
    }

    if ((diffNext == DIRECTION_UP && diffPrev == DIRECTION_RIGHT) || (diffNext == DIRECTION_LEFT && diffPrev == DIRECTION_DOWN)) {
        return SnakeSegmentOrientation::BENT_UP_TO_LEFT;
    }

    std::cout << "ERROR: Using default snake segment" << std::endl;
    return SnakeSegmentOrientation::STRAIGHT_HORIZONTAL;
}

bool Snake::isOnLeftOrRightEdge(const Vector2i &pos) const {
    return pos.x == 0 || pos.x == m_board.getDimensions().x - 1;
}

bool Snake::isOnTopOrBottomEdge(const Vector2i &pos) const {
    return pos.y == 0 || pos.y == m_board.getDimensions().y - 1;
}

void Snake::draw() const {
    // values that change for each segment, will keep using throughout function
    Texture2D textureToDraw = snakeStraightSegmentTexture;
    Vector2 positionInPixels = {0.0f, 0.0f};
    float rotation = 0.0f;

    ////////////////
    //  DRAW HEAD
    ////////////////
    // If snake is just 1 long, draw a stub with a tail (a "derp"), otherwise just draw a head
    textureToDraw = m_segments.size() == 1 ? snakeDerpTexture : snakeHeadTexture;

    positionInPixels = Vector2 {
        static_cast<float>(m_segments.front()->getPosition().x * m_board.getTileDimensions().x),
        static_cast<float>(m_segments.front()->getPosition().y * m_board.getTileDimensions().y)
    };

    rotation = 0.0f;
    if      ( m_headDirection == DIRECTION_UP)      { rotation =     0.0f;  }
    else if ( m_headDirection == DIRECTION_RIGHT)   { rotation =    90.0f;  }
    else if ( m_headDirection == DIRECTION_DOWN)    { rotation =   180.0f;  }
    else if ( m_headDirection == DIRECTION_LEFT)    { rotation =   270.0f;  }
    else { throw std::out_of_range("Snake::draw() rotation direction invalid."); }

    drawSegment(textureToDraw, positionInPixels, rotation);

    //////////////////
    //   DRAW BODY
    //////////////////
    for(int i = 1; i < m_segments.size() - 1; i++) {  // starts on segment after head, and ends on segment before tail
        const auto segmentOrientation = getOrientationOfSegment(i);

        // set texture based on segment orientation
        switch(segmentOrientation) {
            case SnakeSegmentOrientation::STRAIGHT_VERTICAL:
            case SnakeSegmentOrientation::STRAIGHT_HORIZONTAL:
                textureToDraw = snakeStraightSegmentTexture;
                break;
            case SnakeSegmentOrientation::BENT_DOWN_TO_LEFT:
            case SnakeSegmentOrientation::BENT_UP_TO_RIGHT:
            case SnakeSegmentOrientation::BENT_UP_TO_LEFT:
            case SnakeSegmentOrientation::BENT_DOWN_TO_RIGHT:
                textureToDraw = snakeBentSegmentTexture;
                break;
        }

        // set rotation based on segment orientation
        // this is pretty clear-cut, if things are off, look at getSegmentOrientation()
        switch (segmentOrientation) {
            case SnakeSegmentOrientation::STRAIGHT_VERTICAL:    rotation =    0.0f; break;
            case SnakeSegmentOrientation::STRAIGHT_HORIZONTAL:  rotation =   90.0f; break;

            case SnakeSegmentOrientation::BENT_UP_TO_RIGHT:     rotation =    0.0f; break;
            case SnakeSegmentOrientation::BENT_DOWN_TO_RIGHT:   rotation =   90.0f; break;
            case SnakeSegmentOrientation::BENT_DOWN_TO_LEFT:    rotation =  180.0f; break;
            case SnakeSegmentOrientation::BENT_UP_TO_LEFT:      rotation =  270.0f; break;
        }

        positionInPixels = Vector2 {
            static_cast<float>(m_segments[i]->getPosition().x * m_board.getTileDimensions().x),
            static_cast<float>(m_segments[i]->getPosition().y * m_board.getTileDimensions().y)
        };

        drawSegment(textureToDraw, positionInPixels, rotation);
    }

    ////////////
    // DRAW TAIL
    ////////////

     if(m_segments.size() > 1) {
         textureToDraw = snakeTailTexture;
         positionInPixels = Vector2 {
             static_cast<float>(m_segments.back()->getPosition().x * m_board.getTileDimensions().x),
             static_cast<float>(m_segments.back()->getPosition().y * m_board.getTileDimensions().y)
         };

         // This logic mirrors the logic used in the "GetSegmentOrientation" and could potentially be refactored
         // to fall as an edge-case of that function
         auto lastSegmentPos = m_segments.back()->getPosition();
         auto secondToLastSegmentPos = m_segments[m_segments.size() - 1]->getPosition();

         bool anyOnLeftOrRightEdge = isOnLeftOrRightEdge(lastSegmentPos) || isOnLeftOrRightEdge(secondToLastSegmentPos);
         bool anyOnTopOrBottomEdge = isOnTopOrBottomEdge(lastSegmentPos) || isOnTopOrBottomEdge(secondToLastSegmentPos);

         if(anyOnLeftOrRightEdge || anyOnTopOrBottomEdge) {
             const Vector2i TRANSLATE = {
                 anyOnLeftOrRightEdge ? m_board.getDimensions().x / 2 : 0,
                 anyOnTopOrBottomEdge ? m_board.getDimensions().y / 2 : 0
             };

             secondToLastSegmentPos = (secondToLastSegmentPos + TRANSLATE).wrapped(
                 {0, 0},
                 m_board.getDimensions()
            );

             lastSegmentPos = (lastSegmentPos + TRANSLATE).wrapped(
                 {0, 0},
                 m_board.getDimensions()
             );
         }

         const auto diff = lastSegmentPos - secondToLastSegmentPos;

         // Check if the segment is straight (horizontal or vertical)
         if      (diff == DIRECTION_UP)     { rotation =   0.0f; }
         else if (diff == DIRECTION_RIGHT)  { rotation =  90.0f; }
         else if (diff == DIRECTION_DOWN)   { rotation = 180.0f; }
         else if (diff == DIRECTION_LEFT)   { rotation = 270.0f; }

         drawSegment(textureToDraw, positionInPixels, rotation);
     };
}

void Snake::drawSegment(const Texture2D& texture, const Vector2 position, const float rotation) {
    DrawTexturePro (
        texture,
        (Rectangle) {0, 0, 40, 40}, // should refactor to not be literals - relate to tile width
        (Rectangle) {position.x + 20, position.y + 20, 40, 40},
        Vector2{20, 20},
        rotation,
        WHITE
    );
}

void Snake::reset() {
    // empty snake
    m_segments.clear();
    m_segments.shrink_to_fit();

    // just a snake head <3
    const auto RESET_START_POSITION = Vector2i {6, 6};
    m_segments.push_back(std::make_unique<SnakeSegment>(RESET_START_POSITION, GOLD));

}

/// STATE CHANGES ///
void Snake::changeDirection(const Vector2i newDirection) {
    if(!m_headDirection.oppositeTo(newDirection)) {
        m_headDirection = newDirection;
    }
}

bool Snake::selfCollisionOccurred() const {
    for (int i = 1; i < m_segments.size(); ++i) {
        if(m_segments[0]->getPosition() == m_segments[i]->getPosition()) return true;
    }
    return false;
}

bool Snake::hasSegmentAt(Vector2i position) const {
    for(int i = 0; i < m_segments.size(); i++) {
        if(m_segments[i]->getPosition() == position) {
            return true;
        }
    }
    return false;
}





