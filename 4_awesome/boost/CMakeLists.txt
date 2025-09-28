cmake_minimum_required(VERSION 3.20)

project(demo)

# C++标准版本
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# clangd服务使用
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(T_OBJECT ${PROJECT_NAME})
set(ROOT ${PROJECT_SOURCE_DIR}/../..)
set(APP ${PROJECT_SOURCE_DIR})

set(ROOT_INCLUDE ${ROOT}/1_include)
set(ROOT_LIBS ${ROOT}/2_libs)
set(ROOT_BINS ${ROOT}/3_bins)

#================= 上面为公共部分 ================

include_directories(
    ${ROOT_LIBS}/boost/include
    ${ROOT_LIBS}/fmt/include
)
link_libraries(
    ${ROOT_LIBS}/boost/lib/libboost_program_options.so
    ${ROOT_LIBS}/fmt/lib/libfmt.so.10
)


add_executable(po.out program_options.cpp)

