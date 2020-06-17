package com.jing.ffmpegtest;

//NDK方法
public class FFmpegTest {

    //加载动态库
    static {
        System.loadLibrary("native-lib");
    }
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public static native String stringFromJNI();
    public static native void ffmpegTesta();
}