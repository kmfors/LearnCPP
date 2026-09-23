cmake_minimum_required(VERSION 3.20)
project(demo)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/common.cmake)

#============================== 私有部分 ===============================
set(CPPFile 01_main.cpp)
set(APP ${PROJECT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${CPPFile})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${GTEST_DIR}/include
    ${ROOT_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${GTEST_MAIN_LIB}
    ${GTEST_LIB}
)