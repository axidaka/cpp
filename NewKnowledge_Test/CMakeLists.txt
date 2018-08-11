CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(NewKnowledge CXX)

# 分别将头文件和源文件的路径放到 HEAD_FILES SOURCE_FILES变量中
FILE(GLOB_RECURSE  HEAD_FILES   ${CMAKE_CURRENT_SOURCE_DIR}/*.h*)
FILE(GLOB_RECURSE  SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.c*)

SOURCE_GROUP("include" FILES ${HEAD_FILES})
SOURCE_GROUP("src" FILES ${SOURCE_FILES})

# common目录包含
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../common
                    )
# HelperLib目录包含
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../HelperLib/include
                    )

# DataStructHelper目录包含
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../DataStructHelper/include
                    )

# EffectiveC++目录包含
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../EffectiveC++/include
                    )

# WinKernelProgramme目录包含
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../WinKernelProgramme/Include
                    )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR};
                    ${CMAKE_CURRENT_SOURCE_DIR}/Include
                    )

# 输出设置
SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /DDEBUG /MTd /Zi /Ob0 /Od /RTC1")# 直接抄gui的默认，然后 改成mtd
SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG ")

# 编译版本宏定义
add_definitions(-DUNICODE -D_UNICODE -DWIN32 -D_WIN32)  # unicode版本

# Debug下生成文件增加后缀,对exe无效.
#set(CMAKE_DEBUG_POSTFIX "_d")

# 第三方库引用
use_3rd_lib("DirectX" ${Learn_Practise_SOURCE_DIR}/depend/DirectX/Include ${Learn_Practise_SOURCE_DIR}/depend/DirectX/Lib/x86)
use_3rd_lib("zlib" ${Learn_Practise_SOURCE_DIR}/depend/zlib1.2.7/include ${Learn_Practise_SOURCE_DIR}/depend/zlib1.2.7/lib)
use_3rd_lib("jsoncpp" ${Learn_Practise_SOURCE_DIR}/depend/jsoncpp-0.6.00-rc2/include ${Learn_Practise_SOURCE_DIR}/depend/jsoncpp-0.6.00-rc2/lib)
use_3rd_lib("python27" ${Learn_Practise_SOURCE_DIR}/depend/python27/include ${Learn_Practise_SOURCE_DIR}/depend/python27/libs)

# 执行文件生成,CMake语法一定要先生成再链接？
# 如果添加 WIN32 则生成window程序， 应用程序入口是winmain
ADD_EXECUTABLE(NewKnowledge ${HEAD_FILES}; ${SOURCE_FILES})

# 库链接
TARGET_LINK_LIBRARIES(NewKnowledge $<$<CONFIG:Release>:HelperLib.lib>)
TARGET_LINK_LIBRARIES(NewKnowledge $<$<CONFIG:Debug>:HelperLib_d.lib>)

TARGET_LINK_LIBRARIES(NewKnowledge $<$<CONFIG:Release>:EffectiveC++.lib>)
TARGET_LINK_LIBRARIES(NewKnowledge $<$<CONFIG:Debug>:EffectiveC++_d.lib>)

TARGET_LINK_LIBRARIES(NewKnowledge $<$<CONFIG:Release>:WinKernelProgramme.lib>)
TARGET_LINK_LIBRARIES(NewKnowledge $<$<CONFIG:Debug>:WinKernelProgramme_d.lib>)

TARGET_LINK_LIBRARIES(NewKnowledge $<$<CONFIG:Release>:DataStructHelper.lib>)
TARGET_LINK_LIBRARIES(NewKnowledge $<$<CONFIG:Debug>:DataStructHelper_d.lib>)

ADD_DEFINITIONS(-DZLIB_WINAPI)      #使用zlib需要这个宏定义，否则编译会有符号问题
TARGET_LINK_LIBRARIES(NewKnowledge $<$<CONFIG:Release>:zlib1.2.7.lib>)
TARGET_LINK_LIBRARIES(NewKnowledge $<$<CONFIG:Debug>:zlib1.2.7d.lib>)

TARGET_LINK_LIBRARIES(NewKnowledge $<$<CONFIG:Release>:python27.lib>)
TARGET_LINK_LIBRARIES(NewKnowledge $<$<CONFIG:Debug>:python27_d.lib>)

TARGET_LINK_LIBRARIES(NewKnowledge  shlwapi.lib)
#设置链接flags
#SET_TARGET_PROPERTIES(NewKnowledge PROPERTIES LINK_FLAGS /MANIFESTUAC:"level='requireAdministrator' uiAccess='false'")

# 项目依赖设定
add_dependencies(NewKnowledge  DataStructHelper)
add_dependencies(NewKnowledge  HelperLib)
add_dependencies(NewKnowledge  EffectiveC++)
#add_dependencies(NewKnowledge  WinKernelProgramme)