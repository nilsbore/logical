#include "func_evaluator.hpp"
#include <iostream>

template class base_evaluator<double, std::ostream>;
template class var_evaluator<double, std::ostream>;
template class func_evaluator<double, std::ostream>;
