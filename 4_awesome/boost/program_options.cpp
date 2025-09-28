#include "boost/program_options.hpp"
#include "fmt/core.h"
#include <exception>
#include <string>
#include <iostream>

using std::string;
using std::exception;

namespace po = boost::program_options;

struct ParamOption {
    int count = 1;  // 次数
    int speed = 1;  // 延迟时间，秒/次
    string text;    // 文本 
    string file;    // 文件
};

int start(int argc, char* argv[], ParamOption& param) {
    string aile;
    string bile = "hello";
    string cile;

    po::options_description options("Options");
    options.add_options()
        ("help,h",     "demo this help message")
        ("aile,a",     po::value<string>(&aile)->required(),
                        "demo this aile message")
        ("bile,b",     "demo this bile message")
        ("cile,c",     po::value<string>(&cile)->required(),
                        "demo this cile message")
        // 请注意，help 与 h 之间用逗号隔开，且不留空格！
    ;
    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv).options(options).run(), vm);
        po::notify(vm);
    } catch (exception& ex) {
        fmt::print("Error parsing options: {}\n", ex.what());
        std::cout << options << std::endl;
        return 1;
    }
    fmt::print("aile:{}, bile:{}, cile:{}\n", aile, bile, cile);
    return 0;
}


int main(int argc, char* argv[]) {
    ParamOption param;
    if (start(argc, argv, param)) return -1;
    
    return 0;

}