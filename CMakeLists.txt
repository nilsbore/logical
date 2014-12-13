cmake_minimum_required(VERSION 2.8.3)
project(logical)
add_definitions(-O0 -Wall -fPIC -std=c++11)

include_directories(include)

set(LOGIC_HEADERS include/logic/algorithms.hpp include/logic/dense_matrix.hpp include/logic/matrix_functions.hpp include/logic/matrix_operations.hpp
                  include/logic/recursator.hpp include/logic/build_proxy.hpp include/logic/functors.hpp
                  include/logic/scalar.hpp include/logic/string_matrix.hpp include/logic/virtual_matrix.hpp
                  include/logic/constants.hpp include/logic/matrix_factory.hpp include/logic/operator_priorities.hpp include/logic/sparse_matrix.hpp
                  include/logic/upper_triangular.hpp include/logic/virtual_recursator.hpp include/logic/iterator.hpp)

set(UTIL_HEADERS include/util/complex.hpp include/util/convenience.hpp include/util/file.hpp include/util/info_auto_ptr.hpp
                 include/util/list.hpp include/util/map.hpp include/util/string.hpp)

set(CORE_HEADERS include/base_evaluator.hpp include/var_evaluator.hpp include/func_evaluator.hpp)

add_library(logical_core src/logical_core.cpp ${LOGIC_HEADERS} ${UTIL_HEADERS} ${CORE_HEADERS})
add_executable(test_prompt src/test_prompt.cpp)
