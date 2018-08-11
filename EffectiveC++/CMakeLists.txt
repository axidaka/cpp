CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(EffectiveC++ CXX)

# 编译版本宏定义
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)   # unicode版本

# 分别将头文件和源文件的路径放到 HEAD_FILES SOURCE_FILES变量中
FILE(GLOB HEAD_FILES   ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h; ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp;)
FILE(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp; ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c;)

SOURCE_GROUP("include" FILES ${HEAD_FILES})
SOURCE_GROUP("src" FILES ${SOURCE_FILES})

# 增加预编译头，通过vs工程创建的文件都会自动增加预编译文件。
#use_precompiled_header(DataStructHelper
#    "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h"
#    "${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp"
#    )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR};
                    ${CMAKE_CURRENT_SOURCE_DIR}/include
                    )

# 输出设置
SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /DDEBUG /MTd /Zi /Ob0 /Od /RTC1")# 直接抄gui的默认，然后 改成mtd
SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")

set(CMAKE_DEBUG_POSTFIX "_d")# Debug下生成文件增加后缀,对exe无效.

# 生成静态库
ADD_LIBRARY(EffectiveC++ STATIC ${HEAD_FILES};${SOURCE_FILES})