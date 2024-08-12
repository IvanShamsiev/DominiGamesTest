#pragma once

#include "Drawable.h"
#include "Utils.h"

class Cube : public Drawable {
protected:
    GLuint mProgram;
    std::vector<float> arrVertices;
    std::vector<float> MVP;
    float ratio;

    unsigned int VBO, VAO;

    unsigned long long mLastFrameNs = 0;
    float angularVelocity = 0.5;
    float mAngle = 0.0;
public:
    Cube();
    virtual ~Cube() override;
    virtual void draw() override;
    virtual void setMVP(const std::vector<float>& mvp) override;
    virtual void resize(float width, float height) override;
};
