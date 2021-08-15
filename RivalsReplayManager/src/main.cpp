#include <fstream>
#include <iostream>
#include <string>
#include <utf8help/utf8help.hpp>
#include <sstream>
#include "ReplayRecord.hpp"

int main()
{
    std::ifstream in("test.roa", std::ios::binary);
    std::string s;

    if (in.is_open())
    {
        std::ostringstream ss;
        ss << in.rdbuf();
        in.close();
        s = ss.str();

        rrm::ReplayRecord replayRecord(s);
    }
    else
        std::cout << "파일 없음!" << std::endl;


    return 0;
}
