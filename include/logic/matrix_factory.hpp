#ifndef MATRIX_FACTORY_HPP
#define MATRIX_FACTORY_HPP

#include "virtual_matrix.hpp"
#include "sparse_matrix.hpp"
#include "dense_matrix.hpp"
#include "scalar.hpp"
#include "util/info_auto_ptr.hpp"
#include "util/string.hpp"

namespace logic {

	template <typename Value>
    util::info_auto_ptr<virtual_matrix<Value>, util::string> diag(const virtual_matrix<Value>& arg)
	{
        typedef util::info_auto_ptr<virtual_matrix<Value>, util::string> return_type;

		if (!arg.is_vector()) {
			return "Diag argument must be vector.";
		}
		
        return_type rtn = new sparse_matrix<Value>(arg.length(), arg.length());
		
		for (unsigned i = 0; i < arg.length(); ++i) {
			(*rtn)(i, i) = arg[i];
		}
		
		return rtn;
	}
	
	template <typename Value>
    util::info_auto_ptr<virtual_matrix<Value>, util::string> eye(unsigned size) // sätta in i sparse istället?
	{
        typedef util::info_auto_ptr<virtual_matrix<Value>, util::string> return_type;

        return_type rtn = new sparse_matrix<Value>(size, size);
		
		for (unsigned i = 0; i < size; ++i) {
            (*rtn)(i, i) = Value(1);
		}
		
		return rtn;
	}
	
	
} // namespace logic
#endif
