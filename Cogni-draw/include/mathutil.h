#ifndef MATHUTIL_H
#define MATHUTIL_H

#include <glm/glm.hpp>


inline glm::vec2 toNDC(const glm::vec2& px, float w, float h) 
{
    return 
    {
      2.0f * px.x / w - 1.0f,
     -(1.0f - 2.0f * px.y / h)
    };
}

#endif