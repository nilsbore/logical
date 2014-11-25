#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP

#include <cmath>

namespace logic {

	template<typename Value>
	struct Sin {
		
		Value operator() (const Value& value)
		{
			return sin(value);
		}
		
	};
	
	template<typename Value>
	struct Cos {
		
		Value operator() (const Value& value)
		{
			return cos(value);
		}
		
	};
	
	template<typename Value>
	struct Exp {
		
		Value operator() (const Value& value)
		{
			return exp(value);
		}
		
	};
	
	template<typename Value>
	struct Log {
		
		Value operator() (const Value& value)
		{
			return log(value);
		}
		
	};
	
} // namespace logic
#endif