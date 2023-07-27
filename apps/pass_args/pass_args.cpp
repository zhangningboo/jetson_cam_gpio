#include <iostream>
#include "cmdline.h"

#include "fmt/format.h"

void parse_args(cmdline::parser &arg_parser) {
    arg_parser.add<std::string>("lcam", 'l', "left camera port, e.g  /dev/video0", true, "");
    arg_parser.add<std::string>("rcam", 'r', "right camera port, e.g /dev/video2", true, "");
    arg_parser.add<int>("lp", '\0', "left gpio pin, e.g 16", true, 16);
    arg_parser.add<int>("rp", '\0', "right gpio pin, e.g 18", true, 18);
    arg_parser.add<std::string>("save_path", 's', "picture save path", true, "");
}


int main(int argc, char *argv[]) {
    cmdline::parser arg_parser;
    parse_args(arg_parser);
    arg_parser.parse_check(argc, argv);

    std::string save_path = arg_parser.get<std::string>("save_path");
    fmt::print("{}", save_path);

    return 0;
}