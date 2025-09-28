# 三方库的编译与使用说明

编译使用环境：gcc/g++ version 12.2.0 (Debian 12.2.0-14)

## openssl

version: 3.3.2

downloads: https://openssl-library.org/source/index.html

编译命令：

```shell
./Configure --prefix=<openssl-path> --openssldir=<openssl-path>

make -j4 && make install
```

## zlib

version: 1.3.1

downloads: https://github.com/madler/zlib

```shell
mkdir -p build && cd build && rm -rf *

cmake .. -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=<dest_dir>

make -j4 && make install
```

## curl

version: 8.10.1

downloads: https://github.com/curl/curl

```shell
mkdir -p build && cd build && rm -rf *

export OPENSSL_ROOT_DIR=<openssl-path>

cmake .. -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=<dest_dir> \
  -DZLIB_LIBRARY=<zlib-lib-path/libz.so> \
  -DZLIB_INCLUDE_DIR=<zlib-include-path> \
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON

make -j4 && make install
```
注意：curl 库的编译是依赖于 openssl 与 zlib 这两个库的

## protobuf

version: 3.21.12

downloads: https://github.com/protocolbuffers/protobuf

1、编译命令：生成静态库

```shell 
mkdir -p build && cd build && rm -rf *

cmake .. -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=<dest_dir> \
  -Dprotobuf_BUILD_TESTS=OFF \
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON

make -j4 && make install
```
2、编译命令：生成动态库

```shell
cmake .. -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=<dest_dir>
  -Dprotobuf_BUILD_TESTS=OFF \
  -DBUILD_SHARED_LIBS=ON \
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON
```
注意：protobuf 自 3.21.12 版本后将添加依赖 abseil-cpp 库，问题比较多，因此不推荐使用 protobuf 较新版本

## boost

version: 1.86.0

downloads: https://www.boost.org/

```shell
./bootstrap.sh --prefix=<boost-path>

./b2 install
```


## pulsar

version: 3.6.0

downloads: https://github.com/apache/pulsar-client-cpp

```shell
mkdir -p build && cd build && rm -rf *

cmake .. -DCMAKE_BUILD_TYPE=Release \
  -DBoost_INCLUDE_DIR=<boost-include-path> \ 
  -DOPENSSL_INCLUDE_DIR=<openssl-include-path> \
  -DOPENSSL_SSL_LIBRARY=<libssl> \
  -DOPENSSL_CRYPTO_LIBRARY=<libcrypto> \
  -DProtobuf_INCLUDE_DIR=<protobuf-include-path> \
  -DProtobuf_LIBRARIES=<libprotobuf-path> \
  -DCURL_INCLUDE_DIRS=<curl-include-path> \
  -DCURL_LIBRARIES=<libcurl> \
  -DZLIB_INCLUDE_DIRS=<zlib-include-path> \
  -DZLIB_LIBRARIES=<libz> \
  -DBUILD_TESTS=OFF \
  -DCMAKE_INSTALL_PREFIX=<dest_dir>

make -j4 && make install
```
注意：pulsar 的动态库依赖于 openssl、curl、zlib、protobuf、等三方库，并且其动态库链接依赖的静态库时，必须保证依赖的静态库是经过 -fPIC 编译的。CMAKE_POSITION_INDEPENDENT_CODE 确保在编译时生成位置无关代码（PIC）

## rapidjson
version: v1.1.0

downloads: https://github.com/Tencent/rapidjson   http://rapidjson.org/zh-cn/

## json for modern C++
version: 3.11.3

downloads: https://github.com/nlohmann/json

## sqlite3
version: 3.45.0

downloads: https://www.sqlite.org/

sqlite3是C写的以`.c`结尾的文件，因此无法直接参与g++编译，需要先编译为一个动态库。以C编译的动态库是可以直接参与到g++的编译当中的。

编译命令: `gcc -o libsqlite3.so -shared sqlite3.c -fPIC`

sqlite3工具编译命令: `gcc shell.c sqlite3.c -lpthread -ldl -lm -o sqlite3`

工具使用命令: 

```shell
sqlite> .open text.db           # 打开数据库
sqlite> .tables                 # 查看表文件
sqlite> .header on              # 格式化表结构
sqlite> .mode column            # 格式化表结构
sqlite> select * from text.db
```



## spdlog
version: 1.14.1

downloads: https://github.com/gabime/spdlog

具体的编译方式都写在了CMakeLists里，只需要源仓库的include与src的文件

## fmt
version: 10.2.1

downloads: https://github.com/fmtlib/fmt

编译命令: 在源码进行编译 `cmake -DBUILD_SHARED_LIBS=TRUE ..`



