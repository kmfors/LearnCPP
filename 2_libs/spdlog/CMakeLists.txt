cmake_minimum_required(VERSION 3.20)

project(spdlog)

# clangd代码感知需要设置
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 定义这个宏 SPDLOG_COMPILED_LIB 方才可以编译实现文件
add_definitions(-DSPDLOG_COMPILED_LIB)

# 加载头文件
set(SPDLOG_INCLUDE ${PROJECT_SOURCE_DIR}/include)
include_directories(${SPDLOG_INCLUDE})

# 加载指定目录下的所有源文件
aux_source_directory(./src SRC_CODE)

# 生成动态库
add_library(${PROJECT_NAME} SHARED ${SRC_CODE})

# 内置变量默认值为 /usr/local
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
# 可以使用 make install 进行放置
install(TARGETS ${PROJECT_NAME} DESTINATION ..)




# 自动寻找spdlog的包并引入
# if(NOT TARGET spdlog)
#     # Stand-alone build
#     find_package(spdlog REQUIRED)
# endif()

# ---------------------------------------------------------------------------------------
# Example of using pre-compiled library
# ---------------------------------------------------------------------------------------
# add_executable(example example.cpp)
# target_link_libraries(example PRIVATE libspdlog.so)

# ---------------------------------------------------------------------------------------
# Example of using header-only library
# ---------------------------------------------------------------------------------------
# if(SPDLOG_BUILD_EXAMPLE_HO)
#     add_executable(example_header_only example.cpp)
#     target_link_libraries(example_header_only PRIVATE spdlog::spdlog_header_only)
# endif()

