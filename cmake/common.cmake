#============================== 公共部分 ===============================

# 指定CXX版本
set(CMAKE_CXX_STANDARD 17)

# 是否需要CXX版本（若编译器不支持该版本，则直接报错）
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 生成 compile_commands.json 用于代码补全和跳转
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# get_filename_component：路径分解/规范化工具
# CMAKE_CURRENT_LIST_DIR：当前正在处理的 .cmake 文件所在目录
# ABSOLUTE：转成绝对路径
get_filename_component(ROOT ${CMAKE_CURRENT_LIST_DIR}/.. ABSOLUTE)

# 项目根目录 头文件路径
set(ROOT_INCLUDE_DIR ${ROOT}/1_include)

# 项目根目录 三方库路径
set(ROOT_LIBS_DIR    ${ROOT}/2_libs)

# 项目根目录 二进制路径
set(ROOT_BINS_DIR    ${ROOT}/3_bins)

# 项目根目录 proto文件路径
set(ROOT_PROTO_DIR   ${ROOT}/proto)

# gtest 模块路径
set(GTEST_DIR ${ROOT_LIBS_DIR}/googletest)

# gtest 库文件
find_library(GTEST_MAIN_LIB gtest_main PATHS ${GTEST_DIR}/lib NO_DEFAULT_PATH)
find_library(GTEST_LIB      gtest      PATHS ${GTEST_DIR}/lib NO_DEFAULT_PATH)

#=====================================================================
# find_library(<变量名> <库名> [PATHS <路径>...] [NO_DEFAULT_PATH])
# <变量名>：找到后，把库的完整路径存进这个变量。
# <库名>：要找的库名，不带 lib 前缀、不带 .a/.so 后缀。
# PATHS <路径>：去哪找。
# NO_DEFAULT_PATH：只在这些路径找，不去系统默认路径找
#=====================================================================