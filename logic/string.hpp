#ifndef STRING_HPP
#define STRING_HPP

#include "dense_matrix.hpp"

namespace logic {
	
	template <typename Value>
	class string : public dense_matrix<Value> {
	private:
		
		typedef string self;
		typedef dense_matrix<Value> super;
		
	private:
		
		char operator(unsigned y, unsigned x) const
		{
			value_type rtn = (*super::this)(y, x);
			if (rtn >= 'A' && rtn <= 'Z') {
				return (char)(rtn-'A');
			}
			if (rtn >= 'a' && rtn <= 'z') {
				return (char)(rtn-'a');
			}
			return rtn;
		}
		
		char operator[] (unsigned i) const
		{
			value_type rtn = (*super::this)[i];
			if (rtn >= 'A' && rtn <= 'Z') {
				return (char)(rtn-'A');
			}
			if (rtn >= 'a' && rtn <= 'z') {
				return (char)(rtn-'a');
			}
			return rtn;
		}
		
		typedef Value value_type;
		string(const char* str, unsigned length) : super(1, length) {
			for (unsigned i = 0; i < length; ++i) {
				(*this)(1, i) = str[i];
			}
		}

	};
	
} // namespace logic
#endif