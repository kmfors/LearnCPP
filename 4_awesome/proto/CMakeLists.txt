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
set(ROOT_PROTO ${ROOT}/proto)

#================= 上面为公共部分 ================
set(APP_PROTO ${APP}/proto)

execute_process(
    COMMAND ${ROOT_BINS}/protoc --cpp_out=${APP_PROTO} --proto_path=${ROOT_PROTO} biz_message.proto
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error)
if(result)
    #message(WARNING "注意: 需要为protoc程序指定其依赖的动态库路径")
    message(FATAL_ERROR "Error executing command: ${error}")
else()
    #message(STATUS "Output of command: ${output}")
endif()

include_directories(${ROOT_LIBS}/protobuf/include)
aux_source_directory(${APP_PROTO} APP_PROTO_SRC)
set(APP_MAIN_SRC 3_test.cc)

# 生成可执行程序
add_executable(${T_OBJECT} ${APP_MAIN_SRC} ${APP_PROTO_SRC})

# 依赖的头文件
target_include_directories(${T_OBJECT} PRIVATE ${APP_PROTO})
target_include_directories(${T_OBJECT} PRIVATE ${ROOT_INCLUDE}/1_Tools)
target_include_directories(${T_OBJECT} PRIVATE ${ROOT_LIBS}/spdlog/include)

# 依赖的库文件
target_link_libraries(${T_OBJECT} PRIVATE ${ROOT_LIBS}/protobuf/lib/libprotobuf.so)
target_link_libraries(${T_OBJECT} PRIVATE ${ROOT_LIBS}/spdlog/lib/libspdlog.so)



