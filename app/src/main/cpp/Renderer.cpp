#include "Renderer.h"
#include <jni.h>

#include <memory>
#include "Utils.h"
#include "Grid.h"
#include "Cube.h"

Renderer::Renderer() {
    auto pCube = std::make_unique<Cube>();
    auto pGrid = std::make_unique<Grid>();
    arrDrawables.push_back(std::move(pCube));
    arrDrawables.push_back(std::move(pGrid));
}

Renderer::~Renderer() {
    for (auto& drawable : arrDrawables)
        drawable.reset();
    arrDrawables.clear();
}

void Renderer::resize(int w, int h) {
    ratio = float(w)/float(h);

    for (auto& drawable : arrDrawables)
        drawable->resize(w, h);

    std::vector<float> mRotate = getIdentityMatrix(4);
    std::vector<float> mScale = getIdentityMatrix(4);
    std::vector<float> mTranslation = getIdentityMatrix(4);

    getRotationMatrix(mRotate.data(), M_PI/6, 0, 0);
    mScale[0] = 1/ratio;

    std::vector<float> mvp = matrixMultiply(matrixMultiply(mScale, mRotate, 4), mTranslation, 4);

    for (auto& drawable : arrDrawables)
        drawable->setMVP(mvp);

    glViewport(0, 0, w, h);
}

void Renderer::draw() {
    for (auto& drawable : arrDrawables)
        drawable->draw();
}

void Renderer::render() {
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
}

// ----------------------------------------------------------------------------

static std::unique_ptr<Renderer> pRenderer(nullptr);

extern "C" {
JNIEXPORT void JNICALL Java_com_example_dominigamestest_GlesJniLib_init(JNIEnv* env, jobject obj);
JNIEXPORT void JNICALL Java_com_example_dominigamestest_GlesJniLib_resize(JNIEnv* env, jobject obj, jint width, jint height);
JNIEXPORT void JNICALL Java_com_example_dominigamestest_GlesJniLib_step(JNIEnv* env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_example_dominigamestest_GlesJniLib_init(JNIEnv* env, jobject obj) {
    if (pRenderer) {
        pRenderer.reset();
    }
    pRenderer = std::make_unique<Renderer>();
}

JNIEXPORT void JNICALL Java_com_example_dominigamestest_GlesJniLib_resize(JNIEnv* env, jobject obj, jint width, jint height) {
    if (pRenderer) {
        pRenderer->resize(width, height);
    }
}

JNIEXPORT void JNICALL Java_com_example_dominigamestest_GlesJniLib_step(JNIEnv* env, jobject obj) {
    if (pRenderer) {
        pRenderer->render();
    }
}