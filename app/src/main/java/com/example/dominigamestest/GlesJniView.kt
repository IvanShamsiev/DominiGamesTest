package com.example.dominigamestest

import android.content.Context
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class GlesJniView(context: Context?) : GLSurfaceView(context) {
    init {
        // Pick an EGLConfig with RGB8 color, 16-bit depth, no stencil,
        // supporting OpenGL ES 2.0 or later backwards-compatible versions.
        setEGLConfigChooser(8, 8, 8, 0, 16, 0)
        setEGLContextClientVersion(3)
        setRenderer(Renderer())
    }

    private class Renderer : GLSurfaceView.Renderer {
        override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
            GlesJniLib.init()
        }

        override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
            GlesJniLib.resize(width, height)
        }

        override fun onDrawFrame(gl: GL10) {
            GlesJniLib.step()
        }
    }
}