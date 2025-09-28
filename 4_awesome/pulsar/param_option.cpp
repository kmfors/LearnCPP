#include "boost/program_options.hpp"
#include "param_option.h"
#include <exception>
#include <iostream>

using std::exception;
namespace po = boost::program_options;

int main_start(int argc, char* argv[], ParamOption& param) {

    po::options_description options("Options");
    options.add_options()
        ("help,h",     "demo this help message")
        ("topic,t",    po::value<string>(&param.topic), "发送主题")
        //("msg,m",      po::value<string>(&param.msg)->required(), "发送内容")
        ("count,c",    po::value<int>(&param.count), "发送次数")
        ("speed,s",    po::value<int>(&param.speed),"发送时间间隔")
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
    return 0;
}