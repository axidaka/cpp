#-------------------------------------------------------
#这个CMake模块主要用来管理使用的第三方库
#-------------------------------------------------------

# 增加Log4cplus库
#INCLUDE_DIRECTORIES(${MB_SOURCE_DIR}/3rdLib/log4cplus-1.0.4.3\include;)

#use_3rd_lib(库名称 库头文件 库debug版目录 库release版目录)
macro(use_3rd_lib LIB_NAME LIB_INCLUDE LIB_DIR )
    INCLUDE_DIRECTORIES(${LIB_INCLUDE})
    LINK_DIRECTORIES(${LIB_DIR})
endmacro(use_3rd_lib)
