cmake_minimum_required(VERSION 3.20)
project(demo)
set(MAIN_FILE 01_main.cpp)

#============================== 公共部分 ===============================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ROOT ${PROJECT_SOURCE_DIR}/../..)
set(APP ${PROJECT_SOURCE_DIR})

set(ROOT_INCLUDE ${ROOT}/1_include)
set(ROOT_LIBS    ${ROOT}/2_libs)
set(ROOT_BINS    ${ROOT}/3_bins)
set(ROOT_PROTO   ${ROOT}/proto)

set(GTEST_DIR ${ROOT_LIBS}/googletest)

#============================== 私有部分 ===============================

#=====================================================================
# find_library(<变量名> <库名> [PATHS <路径>...] [NO_DEFAULT_PATH])
# <变量名>：找到后，把库的完整路径存进这个变量。
# <库名>：要找的库名，不带 lib 前缀、不带 .a/.so 后缀。
# PATHS <路径>：去哪找。
# NO_DEFAULT_PATH：只在这些路径找，不去系统默认路径找
#=====================================================================
find_library(GTEST_MAIN_LIB gtest_main PATHS ${GTEST_DIR}/lib NO_DEFAULT_PATH)
find_library(GTEST_LIB      gtest      PATHS ${GTEST_DIR}/lib NO_DEFAULT_PATH)

add_executable(${PROJECT_NAME} ${MAIN_FILE})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${GTEST_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${GTEST_MAIN_LIB}     # 入口，依赖 gtest
    ${GTEST_LIB}          # 本体
)