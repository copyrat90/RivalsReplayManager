#include <fstream>
#include <iostream>
#include <string>
#include <utf8help/utf8help.hpp>
#include <sstream>
#include "ReplayRecord.hpp"

int main()
{
    std::ifstream ifs("test.roa", std::ios::binary);
    std::string s;

    if (ifs.is_open())
    {
        std::ostringstream ss;
        ss << ifs.rdbuf();
        ifs.close();
        s = ss.str();

        rrm::ReplayRecord replayRecord(s);
        std::ofstream ofs("out.roa", std::ios::binary);
        if (ofs.is_open())
        {
            ofs << replayRecord.Serialize();
            ofs.close();
        }
        else
            std::cout << "out.roa: file open failed!\n";
    }
    else
        std::cout << "test.roa: file not found!\n";

    return 0;
}
