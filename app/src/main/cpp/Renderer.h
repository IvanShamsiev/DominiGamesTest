#pragma once

#include "Drawable.h"

class Renderer {
public:
    Renderer();
    virtual ~Renderer();
    void resize(int w, int h);
    void render();
protected:

    void draw();
    float ratio = 1.0f;
private:

    std::vector<std::unique_ptr<Drawable>> arrDrawables;
};
