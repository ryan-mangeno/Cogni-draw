#ifndef MATHUTIL_H
#define MATHUTIL_H

#include <glm/glm.hpp>


inline glm::vec2 pixel_to_ndc(const glm::vec2& px, float w, float h) 
{
    return 
    {
      2.0f * px.x / w - 1.0f,
     -(1.0f - 2.0f * px.y / h)
    };
}

inline glm::vec2 ndc_to_pixel(const glm::vec2& ndc, int w, int h) 
{
    return
    {
        std::floor((ndc.x + 1.0f) / 2.0f * w),
        std::ceil((1.0f - ndc.y) / 2.0f * h)
    };
}

#endif