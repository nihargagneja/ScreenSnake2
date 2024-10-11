//
// Created by Nihar Gagneja on 10/7/24.
//

#include "Food.h"

#include <raylib.h>

void Food::draw(Vector2i tileDimensions) const {
    DrawCircle(m_position.x * tileDimensions.x + 20, m_position.y * tileDimensions.y + 20, 10, RED);
}
