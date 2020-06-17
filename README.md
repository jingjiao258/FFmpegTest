环境：
 Android studio 4.0 
 ndk 21.3.6528147
  jdk1.8.0_201

搭建项目：
1，Android studio创建C/C++项目
2，导入.so库，文件结构如图

3，添加、链接动态库,打开 CMakeLists.txt文件:



cmake_minimum_required(VERSION 3.4.1)
# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
# FFMpeg配置
# FFmpeg配置目录
set(distribution_DIR ${CMAKE_SOURCE_DIR}/../../../libs)
#配置编译的头文件
include_directories(${CMAKE_SOURCE_DIR}/../../../libs/include)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
#判断编译器类型,如果是gcc编译器,则在编译选项中加入c++11支持
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)
# 编解码(最重要的库)
add_library(
        avcodec
        SHARED
        IMPORTED)
set_target_properties(
        avcodec
        PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../../../libs/${ANDROID_ABI}/libavcodec.so)
# 设备
add_library(
        avdevice
        SHARED
        IMPORTED)
set_target_properties(
        avdevice
        PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../../../libs/${ANDROID_ABI}/libavdevice.so)
# 滤镜特效处理库
add_library(
        avfilter
        SHARED
        IMPORTED)
set_target_properties(
        avfilter
        PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../../../libs/${ANDROID_ABI}/libavfilter.so)
# 封装格式处理库
add_library(
        avformat
        SHARED
        IMPORTED)
set_target_properties(
        avformat
        PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../../../libs/${ANDROID_ABI}/libavformat.so)
# 工具库(大部分库都需要这个库的支持)
add_library(
        avutil
        SHARED
        IMPORTED)
set_target_properties(
        avutil
        PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../../../libs/${ANDROID_ABI}/libavutil.so)
add_library(
        postproc
        SHARED
        IMPORTED)
set_target_properties(
        postproc
        PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../../../libs/${ANDROID_ABI}/libpostproc.so)
#  音频采样数据格式转换库
add_library(
        swresample
        SHARED
        IMPORTED)
set_target_properties(
        swresample
        PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../../../libs/${ANDROID_ABI}/libswresample.so)
#   视频像素数据格式转换
add_library(
        swscale
        SHARED
        IMPORTED)
set_target_properties(
        swscale
        PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../../../libs/${ANDROID_ABI}/libswscale.so)




## 引入FFmpeg的库文件，设置内部的方式引入，指定库的目录是 -L  指定具体的库-l
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/${CMAKE_ANDROID_ARCH_ABI}")
add_library( # Sets the name of the library.
             native-lib
             # Sets the library as a shared library.
             SHARED
             # Provides a relative path to your source file(s).
             native-lib.cpp )


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
find_library( # Sets the name of the path variable.
              log-lib
              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )
# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.




target_link_libraries( # Specifies the target library.
        native-lib
        ${log-lib}
        -Wl,--start-group
        avcodec avdevice avfilter avformat avutil postproc swresample swscale
        -Wl,--end-group
        android
                       # Links the target library to the log library
                       # included in the NDK.
                        )



注意;${CMAKE_SOURCE_DIR}表示的是CMakeLists.txt文件所在的位置，通过此文件位置确定其他的文件的相对位置

问题参考：
1，编译的时候不通过，报错 error: xxx.so，needed by xxxx.so，missing and no known rule to make it

这个错误的意思是你生成xxxx.so的时候，需要xxx.so库，但是没有找到，其实在这里就是路径的问题，在CMake的使用中，可以通过add_library依赖第三方库

set_target_properties(
        avcodec
        PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../../../libs/${ANDROID_ABI}/libavcodec.so)

     上面代码avcodec 其实就是依赖库的名字，可以随便取，但是下面set_target_properties的第一个参数一定要和上面统一，具体的参数含义就不在多说了，后面的

${CMAKE_SOURCE_DIR}/../../../libs/${ANDROID_ABI}/libavcodec.so

就是你依赖的so库的存放路径，${CMAKE_SOURCE_DIR} 其实就是CMakeLists.txt文件所在文件夹，${ANDROID_ABI}其实就是你编译的手机的cpu架构，比如armeabi、X86、mips64等等，编译的时候，它会自动去找libs的对应文件夹，如果找不到，就会报这个错误。现在大部分手机都是armeabi架构，模拟器是x86架构，所以如果出现这个错误，需要检查一下自己so库是否是对应版本，以及是否存放在了对应文件夹下。
参考
 https://blog.csdn.net/qqchenjian318/article/details/72780874?locationNum=3&fps=1
 
4,配置CPU平台架构类型
在app->src->build.gradle中添加配置:abiFilters 'arm64-v8a'

其中abiFilters就指定了需要打哪种架构的so库
整个build.gradle结构


apply plugin: 'com.android.application'


android {
    compileSdkVersion 28
    buildToolsVersion "30.0.0"


    defaultConfig {
        applicationId "com.jing.ffmpegtest"
        minSdkVersion 21
        targetSdkVersion 28
        versionCode 1
        versionName "1.0"


        testInstrumentationRunner "android.support.test.runner.AndroidJUnitRunner"


        externalNativeBuild {
            cmake {
                abiFilters 'arm64-v8a'
            }
        }
        ndk{
            abiFilters 'arm64-v8a'
        }
    }
    sourceSets{
        main{
            jniLibs.srcDirs = ["libs"];
        }
    }
    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android-optimize.txt'), 'proguard-rules.pro'
        }
    }
    externalNativeBuild {
        cmake {
            path "src/main/cpp/CMakeLists.txt"
            version "3.10.2"
        }
    }
    compileOptions {
        sourceCompatibility JavaVersion.VERSION_1_8
        targetCompatibility JavaVersion.VERSION_1_8
    }
    packagingOptions {
        pickFirst 'lib/armeabi-v7a/libavcodec.so'
        pickFirst 'lib/armeabi-v7a/libavutil.so'
        pickFirst 'lib/armeabi-v7a/libswscale.so'
        pickFirst 'lib/armeabi-v7a/libswresample.so'
        pickFirst 'lib/armeabi-v7a/libpostproc.so'
        pickFirst 'lib/armeabi-v7a/libavformat.so'
        pickFirst 'lib/armeabi-v7a/libavfilter.so'
        pickFirst 'lib/armeabi-v7a/libavdevice.so'


        pickFirst 'lib/arm64-v8a/libavcodec.so'
        pickFirst 'lib/arm64-v8a/libavutil.so'
        pickFirst 'lib/arm64-v8a/libswscale.so'
        pickFirst 'lib/arm64-v8a/libswresample.so'
        pickFirst 'lib/arm64-v8a/libpostproc.so'
        pickFirst 'lib/arm64-v8a/libavformat.so'
        pickFirst 'lib/arm64-v8a/libavfilter.so'
        pickFirst 'lib/arm64-v8a/libavdevice.so'
    }
}


dependencies {
    implementation fileTree(dir: "libs", include: ["*.jar"])
    implementation 'com.android.support:appcompat-v7:28.0.0'
    implementation 'com.android.support.constraint:constraint-layout:1.1.3'
    testImplementation 'junit:junit:4.12'
    androidTestImplementation 'com.android.support.test:runner:1.0.2'
    androidTestImplementation 'com.android.support.test.espresso:espresso-core:3.0.2'


}
5，编写案例与测试
创建FFmpegTest.java


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
定义C/C++方法（NDK方法和Java方法必须一一对应）
注意：在CMakeLists.txt配置native-lib.cpp文件,将Java和C/C++进行关联

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
注意 C/C++ 源文件引入


extern "C"{
#include <....>
}


调用测试

FFmpegTest.ffmpegTesta();

注意在项目运行时会出现以下错误


More than one file was found with OS independent path 'lib/armeabi/xxx.so'原因是 
多个依赖库中在lib目录包含了相同的so文件.
解决方法
在主工程的build.gradle中,android{}中创建packagingOptions. 提示哪个so文件重复了，就加一个pickFirst 后面的引号中加上报错的文件路径.


android{  
............
 packagingOptions {
        pickFirst 'lib/arm64-v8a/libavcodec.so'
        pickFirst 'lib/arm64-v8a/libavutil.so'
        pickFirst 'lib/arm64-v8a/libswscale.so'
        pickFirst 'lib/arm64-v8a/libswresample.so'
        pickFirst 'lib/arm64-v8a/libpostproc.so'
        pickFirst 'lib/arm64-v8a/libavformat.so'
        pickFirst 'lib/arm64-v8a/libavfilter.so'
        pickFirst 'lib/arm64-v8a/libavdevice.so'
    }
}