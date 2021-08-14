#include <string>
#include <utf8help/utf8help.hpp>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>

int main()
{
    std::string s1 = "こんにちは Привет Hello 안녕하세요";
    std::string s2 = utf8help::Upper(s1);

    nana::form fm;
    nana::label lb1(fm, nana::rectangle{ 10,10,100,100 });
    lb1.caption(s1);
    nana::label lb2(fm, nana::rectangle{ 100,100,100,100 });
    lb2.caption(s2);

    fm.show();
    nana::exec();

    return 0;
}
