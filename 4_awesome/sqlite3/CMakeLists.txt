cmake_minimum_required(VERSION 3.20)

project(demo)

set(ROOT ${PROJECT_SOURCE_DIR})
set(ROOT_INCLUDE ${PROJECT_SOURCE_DIR}/../../1_include)
set(ROOT_LIBS ${PROJECT_SOURCE_DIR}/../../2_thirdLibs)

#include_directories(${ROOT_PROTO})
# 加载指定目录下的所有源文件
#aux_source_directory(${ROOT_PROTO} SRC_CODE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable(${PROJECT_NAME} 1_test.cc )


target_include_directories(${PROJECT_NAME} PRIVATE ${ROOT_INCLUDE}/sqlite3)

target_link_libraries(${PROJECT_NAME} ${ROOT_LIBS}/libsqlite3.so)
