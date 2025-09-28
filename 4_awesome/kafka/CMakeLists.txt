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


set(APP_MAIN_SRC pro.cc)

include_directories(
    ${ROOT_LIBS}/rdkafka/include
    ${ROOT_LIBS}/cppkafka/include
    ${ROOT_LIBS}/boost/include)

link_libraries(
    ${ROOT_LIBS}/cppkafka/lib/libcppkafka.so
    ${ROOT_LIBS}/rdkafka/lib/librdkafka.so.1
    ${ROOT_LIBS}/rdkafka/lib/librdkafka++.so.1
    ${ROOT_LIBS}/boost/lib/libboost_program_options.so
    )

# 生成可执行程序
add_executable(con con.cc)

add_executable(pro pro.cc)

# 依赖的头文件

#target_include_directories(${T_OBJECT} PRIVATE ${ROOT_LIBS}/librdkafka/include)


# 依赖的库文件
#target_link_libraries(${T_OBJECT} PRIVATE ${ROOT_LIBS}/cppkafka/lib/libcppkafka.so)
#target_link_libraries(${T_OBJECT} PRIVATE ${ROOT_LIBS}/librdkafka/lib/librdkafka.so.1)
#target_link_libraries(${T_OBJECT} PRIVATE ${ROOT_LIBS}/librdkafka/lib/librdkafka++.so.1)
#target_link_libraries(${T_OBJECT} PRIVATE ${ROOT_LIBS}/boost/lib/libboost_program_options.so)


