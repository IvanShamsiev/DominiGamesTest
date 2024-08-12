#pragma once

#include <GLES3/gl32.h>
#include <vector>

class Drawable {
public:
    Drawable() = default;
    virtual ~Drawable() = default;
    virtual void draw() = 0;
    virtual void setMVP(const std::vector<float>& mvp) = 0;
    virtual void resize(float width, float height) = 0;
};
