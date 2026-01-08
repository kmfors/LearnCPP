# ================================================================
# Buffer Project CMake 配置 (跨平台)
# ================================================================

cmake_minimum_required(VERSION 3.10)
project(buffer_test CXX)
set(CKCU_EXECUTE_TARGER ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-g -O0)
else()
  add_compile_options(-O2)
endif()

# 控制执行目标文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 源文件
file(GLOB SRCS "*.cpp")

add_executable(${CKCU_EXECUTE_TARGER} ${SRCS})

if(WIN32)
  target_link_libraries(${CKCU_EXECUTE_TARGER} ws2_32)
else()
endif()

# 便捷目标
add_custom_target(run
  COMMAND $<TARGET_FILE:${CKCU_EXECUTE_TARGER}>
  DEPENDS ${CKCU_EXECUTE_TARGER}
)

# 配置说明
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Output dir: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# CMAKE_RUNTIME_OUTPUT_DIRECTORY 设置所有可执行文件（add_executable 创建的目标）的输出目录
# CMAKE_RUNTIME_OUTPUT_DIRECTORY 通常就是搭配 CMAKE_BINARY_DIR 一起使用的，获取绝对路径
# $<TARGET_FILE:buffer_test> 编译阶段获取目标文件的完整路径

# file(GLOB HDRS "*.h")        # 所有 .h 文件
# file(GLOB <variable> <pattern>) 全局模式匹配
# file(READ ...)	读取文件内容
# file(WRITE ...)	写入文件
# file(GLOB ...)	通配符匹配（不递归）
# file(GLOB_RECURSE ...)	通配符匹配（递归）
# cmake -G "MinGW Makefiles" -B ./build
# cmake --build .\build\