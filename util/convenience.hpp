#ifndef CONVENIENCE_HPP
#define CONVENIENCE_HPP

#include "complex.hpp"

namespace util {
	
	template <typename Value>
	struct is_complex {
		const static bool value = false;
	};
	
	template <typename Value>
	struct is_complex<complex<Value> > {
		const static bool value = true;
	};
	
	template <typename Value>
	struct get_base_type {
		typedef Value type;
	};
	
	template <typename Value>
	struct get_base_type<complex<Value> > {
		typedef Value type;
	};
	
	template <typename Value>
	Value get_real(Value c) {
		if (is_complex<Value>::value) {
			return re(c);
		}
		return c;
	}
	
	template <typename Value>
	Value get_imag(Value c) {
		if (is_complex<Value>::value) {
			return im(c);
		}
		return Value(0);
	}
	
	template <typename Type>
	struct is_const {
		const static bool value = false;
	};
	
	template <typename Type>
	struct is_const<const Type> {
		const static bool value = true;
	};
	
	template <bool Value, typename Type1, typename Type2>
	struct if_c {
		typedef Type2 type;
	};
	
	template <typename Type1, typename Type2>
	struct if_c<true, Type1, Type2> {
		typedef Type1 type;
	};
	
	template <typename Condition, typename Type1, typename Type2>
	struct if_ {
		typedef typename if_c<Condition::value, Type1, Type2>::type type;
	};
	
	template <typename Type>
	struct is_void {
		const static bool value = false;
	};
	
	template <>
	struct is_void<void> {
		const static bool value = true;
	};
	
} // namespace logic
#endif