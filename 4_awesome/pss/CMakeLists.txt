cmake_minimum_required(VERSION 3.20)

project(PSS)

# 开启编译命令
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ROOT ${PROJECT_SOURCE_DIR}/../../)
set(ROOT_INCLUDE ${ROOT}/1_include)
set(ROOT_LIBS ${ROOT}/2_thirdLibs)

#include_directories(${ROOT_PROTO})
# 加载指定目录下的所有源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/src PROJECT_CODE)

add_executable(${PROJECT_NAME} ${PROJECT_CODE})


target_include_directories(${PROJECT_NAME} PRIVATE ${ROOT_INCLUDE}/spdlog)
target_include_directories(${PROJECT_NAME} PRIVATE ${ROOT_INCLUDE}/1_Tools)

target_link_libraries(${PROJECT_NAME} ${ROOT_LIBS}/libspdlog.so)
