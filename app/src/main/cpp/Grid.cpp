#include "Grid.h"
#include "Utils.h"

static const char VERTEX_SHADER[] =
        "#version 300 es\n"
        "layout (location = 0) in vec3 vPosition;\n"
        "uniform mat4 MVP;\n"
        "void main() {\n"
        "    gl_Position = vec4(vPosition, 1.0);\n"
        "    gl_Position = MVP * gl_Position;\n"
        "}\n";

static const char FRAGMENT_SHADER[] =
        "#version 300 es\n"
        "precision mediump float;\n"
        "uniform vec4 color;\n"
        "out vec4 outColor;\n"
        "void main() {\n"
        "    outColor = color;\n"
        "}\n";

Grid::Grid() : Drawable(), ratio(1.0) {
    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);


    float lineSize = 1.8f;
    linesCount = 20;
    for (uint i = 0; i < linesCount; ++i) {
        float coordOffset = lineSize/2 - lineSize/(linesCount-1) * float(i);

        // vertical line
        vertices.push_back(coordOffset);
        vertices.push_back(0.0f);
        vertices.push_back(-lineSize/2);
        vertices.push_back(coordOffset);
        vertices.push_back(0.0f);
        vertices.push_back(lineSize/2);

        // horizontal line
        vertices.push_back(-lineSize/2);
        vertices.push_back(0.0f);
        vertices.push_back(coordOffset);
        vertices.push_back(lineSize/2);
        vertices.push_back(0.0f);
        vertices.push_back(coordOffset);
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Grid::draw() {
    glUseProgram(mProgram);

    auto mvpHandle = glGetUniformLocation(mProgram, "MVP");
    glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, MVP.data());
    auto colorHandle = glGetUniformLocation(mProgram, "color");
    glUniform4fv(colorHandle, 1, &lineColor[0]);

    glBindVertexArray(VAO);
    for (int i = 0; i < linesCount*2; ++i)
        glDrawArrays(GL_LINES, i*2, 2);
}

void Grid::setMVP(const std::vector<float>& mvp) {
    MVP = mvp;
}

void Grid::resize(float width, float height) {
    ratio = width/height;
}

Grid::~Grid() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(mProgram);
}
