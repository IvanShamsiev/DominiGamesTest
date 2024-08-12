#include "Cube.h"

#define PI2 (M_PI * 2)

static const float colors[6][4] = {
        {1.0f, 0.5f, 0.0f, 1.0f},  // 0. orange
        {1.0f, 0.0f, 1.0f, 1.0f},  // 1. violet
        {0.0f, 1.0f, 0.0f, 1.0f},  // 2. green
        {0.0f, 0.0f, 1.0f, 1.0f},  // 3. blue
        {1.0f, 0.0f, 0.0f, 1.0f},  // 4. red
        {1.0f, 1.0f, 0.0f, 1.0f}   // 5. yellow
};

static const int NUM_FACES = 6;
static const int COORDS_PER_VERTEX = 3;
static float vertices[] = {
        // FRONT
        -0.2f, -0.2f,  0.2f,  // 0. left-bottom-front
        0.2f, -0.2f,  0.2f,  // 1. right-bottom-front
        -0.2f,  0.2f,  0.2f,  // 2. left-top-front
        0.2f,  0.2f,  0.2f,  // 3. right-top-front
        // BACK
        0.2f, -0.2f, -0.2f,  // 6. right-bottom-back
        -0.2f, -0.2f, -0.2f,  // 4. left-bottom-back
        0.2f,  0.2f, -0.2f,  // 7. right-top-back
        -0.2f,  0.2f, -0.2f,  // 5. left-top-back
        // LEFT
        -0.2f, -0.2f, -0.2f,  // 4. left-bottom-back
        -0.2f, -0.2f,  0.2f,  // 0. left-bottom-front
        -0.2f,  0.2f, -0.2f,  // 5. left-top-back
        -0.2f,  0.2f,  0.2f,  // 2. left-top-front
        // RIGHT
        0.2f, -0.2f,  0.2f,  // 1. right-bottom-front
        0.2f, -0.2f, -0.2f,  // 6. right-bottom-back
        0.2f,  0.2f,  0.2f,  // 3. right-top-front
        0.2f,  0.2f, -0.2f,  // 7. right-top-back
        // TOP
        -0.2f,  0.2f,  0.2f,  // 2. left-top-front
        0.2f,  0.2f,  0.2f,  // 3. right-top-front
        -0.2f,  0.2f, -0.2f,  // 5. left-top-back
        0.2f,  0.2f, -0.2f,  // 7. right-top-back
        // BOTTOM
        -0.2f, -0.2f, -0.2f,  // 4. left-bottom-back
        0.2f, -0.2f, -0.2f,  // 6. right-bottom-back
        -0.2f, -0.2f,  0.2f,  // 0. left-bottom-front
        0.2f, -0.2f,  0.2f   // 1. right-bottom-front
};
static const short indices[] = {0,1,2,2,1,3,5,4,7,7,4,6,8,9,10,10,9,11,12,13,14,14,13,15,16,17,18,18,17,19,22,23,20,20,23,21};


static const char VERTEX_SHADER[] =
        "#version 300 es\n"
        "in vec4 vPosition;\n"
        "uniform mat4 MVP;\n"
        "uniform mat4 mRotate;\n"
        "in vec4 offset;\n"
        "in vec4 color;\n"
        "out vec4 vColor;\n"
        "void main() {\n"
        "    gl_Position = mRotate * vPosition + offset;\n"
        "    gl_Position = MVP * gl_Position;\n"
        "    vColor = color;\n"
        "}\n";

static const char FRAGMENT_SHADER[] =
        "#version 300 es\n"
        "precision mediump float;\n"
        "in vec4 vColor;\n"
        "out vec4 outColor;\n"
        "void main() {\n"
        "    outColor = vColor;\n"
        "}\n";

Cube::Cube() : Drawable() {
    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (!mProgram) return;
    glEnable(GL_DEPTH_TEST);

    for (float i : vertices) arrVertices.push_back(i);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, arrVertices.size() * sizeof(float), arrVertices.data(), GL_STATIC_DRAW);

    auto positionHandle = glGetAttribLocation(mProgram, "vPosition");
    glEnableVertexAttribArray(positionHandle);
    glVertexAttribPointer(positionHandle, COORDS_PER_VERTEX, GL_FLOAT, GL_FALSE, COORDS_PER_VERTEX * sizeof(float), (void*)0);

}

void Cube::draw() {
    glUseProgram(mProgram);

    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    auto nowNs = now.tv_sec * 1000000000ull + now.tv_nsec;
    if (mLastFrameNs == 0) {
        mLastFrameNs = nowNs;
        return;
    }

    float dt = float(nowNs - mLastFrameNs) * 0.000000001f;
    mLastFrameNs = nowNs;

    mAngle += angularVelocity * dt;
    if (mAngle >= PI2) {
        mAngle -= PI2;
    } else if (mAngle <= -PI2) {
        mAngle += PI2;
    }

    float rotMatrix[16];
    getRotationMatrix(rotMatrix, 0, mAngle, 0);
    auto rotMatrixHandle = glGetUniformLocation(mProgram, "mRotate");
    glUniformMatrix4fv(rotMatrixHandle, 1, GL_FALSE, rotMatrix);

    const float offset[] = { sin(mAngle) * 0.6f, 0, cos(mAngle) * 0.6f, 0};
    auto offsetHandle = glGetAttribLocation(mProgram, "offset");
    glVertexAttrib4fv(offsetHandle, offset);

    auto mvpHandle = glGetUniformLocation(mProgram, "MVP");
    glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, MVP.data());

    glBindVertexArray(VAO);
    for (int face = 0; face < NUM_FACES; face++) {
        auto colorHandle = glGetAttribLocation(mProgram, "color");
        glVertexAttrib4fv(colorHandle, colors[face]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, &indices[face * 6]);
    }
}

void Cube::setMVP(const std::vector<float> &mvp) {
    MVP = mvp;
}

void Cube::resize(float width, float height) {
    ratio = width / height;
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(mProgram);
}
