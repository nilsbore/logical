#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "virtual_matrix.hpp"
#include "dense_matrix.hpp"
#include "sparse_matrix.hpp"

namespace logic {
	
	template <typename Value>
	virtual_matrix<Value>* product(const virtual_matrix<Value>& first,
								   const virtual_matrix<Value>& second)
	{
		virtual_matrix<Value>* rtn;
		
		if (first.is_scalar()) {
			rtn = second.clone();
			(*rtn) *= first;
		}
		else if (first.is_sparse()) {
			sparse_matrix<Value>& s =
			static_cast<const sparse_matrix<Value> (first);
			if (second.is_scalar()) {
				return new scalar(first(0, 0)*second(0, 0));
			}
			else if (second.is_sparse()) {
				sparse_matrix<Value>& n =
				static_cast<const sparse_matrix<Value> (second);
				return product(s, n);
			}
			else {
				dense_matrix<Value>& n =
				static_cast<const dense_matrix<Value> (second);
				return product(s, n);				 
			}
		}
		else {
			sparse_matrix<Value>& s =
			static_cast<const sparse_matrix<Value> (first);
			if (second.is_scalar()) {
				return new scalar(first(0, 0)*second(0, 0));
			}
			else if (second.is_sparse()) {
				sparse_matrix<Value>& n =
				static_cast<const sparse_matrix<Value> (second);
				return product(s, n);
			}
			else {
				dense_matrix<Value>& n =
				static_cast<const dense_matrix<Value> (second);
				return product(s, n);				 
			}
		}
	}
	
} // namespace logic
#endif