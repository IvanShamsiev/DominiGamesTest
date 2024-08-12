#pragma once

#include "Drawable.h"

class Grid : public Drawable {
protected:
    GLuint mProgram;
    std::vector<float> MVP;
    std::vector<float> vertices;
    uint linesCount;
    unsigned int VBO, VAO;
    float ratio;

    float lineColor[4] = {0.0f, 1.0f, 0.0f, 1.0f};
public:
    Grid();
    virtual ~Grid() override;
    virtual void draw() override;
    virtual void setMVP(const std::vector<float>& mvp) override;
    virtual void resize(float width, float height) override;
};
