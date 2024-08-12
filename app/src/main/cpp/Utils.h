#pragma once

#include <android/log.h>
#include <set>
#include <GLES3/gl32.h>

#define LOG_TAG "DOMINI_GAMES_TEST"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)


static std::vector<float> getIdentityMatrix(int dim) {
    std::vector<float> mat(dim*dim);
    for (int i = 0; i < dim*dim; ++i)
        mat[i] = 0.0f;
    for (int i = 0; i < dim; ++i)
        mat[i*dim + i] = 1.0f;
    return mat;
}

static std::vector<float> matrixMultiply(std::vector<float> first, std::vector<float> second, int dim) {
    int r1, r2, c1, c2;
    r1 = r2 = c1 = c2 = dim;

    std::vector<float> result(dim*dim);
    for(int i = 0; i < r1; ++i) {
        for (int j = 0; j < c2; ++j) {
            result[i*dim + j] = 0;
        }
    }

    // Multiplying matrix a and b and storing in array mult.
    for(int i = 0; i < r1; ++i) {
        for(int j = 0; j < c2; ++j) {
            for (int k = 0; k < c1; ++k) {
                result[i*dim + j] += first[i*dim + k] * second[k*dim + j];
            }
        }
    }
    return result;
}

static void getRotationMatrix(float* mat, float angle_x, float angle_y, float angle_z) {
    float cx      = cos(angle_x);
    float sx      = sin(angle_x);
    float cy      = cos(angle_y);
    float sy      = sin(angle_y);
    float cz      = cos(angle_z);
    float sz      = sin(angle_z);

    float cxsy     = cx * sy;
    float sxsy     = sx * sy;

    mat[0] = cy * cz;
    mat[1] = -cy * sz;
    mat[2] =  -sy;
    mat[4] = -sxsy * cz + cx * sz;
    mat[5] = sxsy * sz + cx * cz;
    mat[6] = -sx * cy;
    mat[8] = cxsy * cz + sx * sz;
    mat[9] = -cxsy * sz + sx * cz;
    mat[10]= cx * cy;

    mat[3] =  mat[7] = mat[11] = mat[12] = mat[13] = mat[14] = 0;
    mat[15]=  1;
}

static bool checkGlError(const char* funcName) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        ALOGE("GL error after %s(): 0x%08x\n", funcName, err);
        return true;
    }
    return false;
}

static GLuint createShader(GLenum shaderType, const char* src) {
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        checkGlError("glCreateShader");
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                ALOGE("Could not compile %s shader:\n%s\n",
                      shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment", infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

static GLuint createProgram(const char* vtxSrc, const char* fragSrc) {
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    auto cleanAndReturn = [&]() -> GLuint {
        glDeleteShader(vtxShader);
        glDeleteShader(fragShader);
        return program;
    };

    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader) return cleanAndReturn();

    fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader) return cleanAndReturn();

    program = glCreateProgram();
    if (!program) {
        checkGlError("glCreateProgram");
        return cleanAndReturn();
    }
    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) { // if program is not linked, print msg and return
        ALOGE("Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                ALOGE("Could not link program:\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }

    return cleanAndReturn();
}
