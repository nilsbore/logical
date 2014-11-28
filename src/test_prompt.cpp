#include <iostream>
#include <func_evaluator.hpp>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    var_evaluator<double, ostream> eval(cout);
    typedef base_evaluator<double, ostream>::return_type return_type;
    std::string s;
    do {
        std::cout << ">> ";
        //std::cin >> std::noskipws >> s;
        std::getline(std::cin, s);
        //std::cout << s << std::endl;
        //s = std::string("1 2 3 \\ 4 5 6");
        return_type rtn = eval.evaluate(s.c_str(), s.size());
    }
    while (s != "quit");
    return 0;
}
