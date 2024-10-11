//
// Created by Nihar Gagneja on 10/7/24.
//

#ifndef FOOD_H
#define FOOD_H
#include "Vector2i.h"


class Food {
private:
    Vector2i m_position = {0, 0};
public:
    [[nodiscard]] Vector2i getPosition() const { return m_position; }
    void setPosition(Vector2i position) { m_position = position; }
    void draw(Vector2i tileDimensions) const;
};



#endif //FOOD_H
