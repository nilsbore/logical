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
        std::cin >> s;
        //s = std::string("4+4");
        return_type rtn = eval.evaluate(s.c_str(), s.size());
    }
    while (s != "quit");
    return 0;
}
