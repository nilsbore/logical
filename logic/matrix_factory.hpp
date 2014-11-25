#ifndef MATRIX_FACTORY_HPP
#define MATRIX_FACTORY_HPP

#include "virtual_matrix.hpp"
#include "sparse_matrix.hpp"
#include "dense_matrix.hpp"
#include "scalar.hpp"

namespace logic {
	
	template <typename Value>
	return_type<Value> diag(const virtual_matrix<Value>& arg)
	{
		if (!arg.is_vector()) {
			return "Diag argument must be vector.";
		}
		
		return_type<Value> rtn = new sparse_matrix<Value>(arg.length(), arg.length());
		
		for (unsigned i = 0; i < arg.length(); ++i) {
			(*rtn)(i, i) = arg[i];
		}
		
		return rtn;
	}
	
	template <typename Value>
	return_type<Value> eye(unsigned size) // sätta in i sparse istället?
	{
		return_type<Value> rtn = new sparse_matrix<Value>(size, size);
		
		for (unsigned i = 0; i < size; ++i) {
			rtn(i, i) = Value(1);
		}
		
		return rtn;
	}
	
	
} // namespace logic
#endif