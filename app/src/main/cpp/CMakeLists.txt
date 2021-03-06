# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

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