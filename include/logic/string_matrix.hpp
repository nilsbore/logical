#ifndef STRING_MATRIX_HPP
#define STRING_MATIRX_HPP

#include "dense_matrix.hpp"
#include "../util/string.hpp"

namespace logic {
	
	template <typename Value>
	class string_matrix : public dense_matrix<Value> {
	private:
		
		typedef string_matrix self;
		
	public:
		
		typedef virtual_matrix<Value> super;
		typedef Value value_type;
		
		string_matrix(const util::string& str) : dense_matrix<value_type>(1, str.length(),
																		  super::STRING)
		{
			for (unsigned i = 0; i < str.length(); ++i) {
				(*this)(0, i) = (value_type)str[i];
			}
		}
	};

} // namespace logic
#endif