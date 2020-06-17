#include <jni.h>
#include <string>
#include <android/log.h>

extern "C" {
//avcodec:编解码(最重要的库)
#include <libavcodec/avcodec.h>
//avformat:封装格式处理
#include "libavformat/avformat.h"


extern "C"
JNIEXPORT void JNICALL
Java_com_jing_ffmpegtest_FFmpegTest_ffmpegTesta(JNIEnv *env, jclass clazz) {
    // TODO: implement ffmpegTesta()
    const  char *configuration=avcodec_configuration();
    __android_log_print(ANDROID_LOG_INFO,"ffmpeg配置信息","%s",configuration);
}
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_jing_ffmpegtest_FFmpegTest_stringFromJNI(JNIEnv *env, jclass clazz) {
    // TODO: implement stringFromJNI()
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}