package com.example.dominigamestest

object GlesJniLib {

    init {
        System.loadLibrary("dominigamestest")
    }

    external fun init()
    external fun resize(width: Int, height: Int)
    external fun step()
}
