#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP

#include <cmath>
#include "util/complex.hpp"

namespace logic {

	template<typename Value>
	struct Sin {
		
		Value operator() (const Value& value)
		{
			return sin(value);
		}
		
	};

    template<typename Value>
    struct Sin<util::complex<Value> > {

        util::complex<Value> operator() (const util::complex<Value>& value)
        {
            return sin(re(value));
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
    struct Cos<util::complex<Value> > {

        util::complex<Value> operator() (const util::complex<Value>& value)
        {
            return cos(re(value));
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
    struct Exp<util::complex<Value> > {

        util::complex<Value> operator() (const util::complex<Value>& value)
        {
            return exp(re(value));
        }

    };
	
	template<typename Value>
	struct Log {
		
		Value operator() (const Value& value)
		{
			return log(value);
		}
		
	};

    template<typename Value>
    struct Log<util::complex<Value> > {

        util::complex<Value> operator() (const util::complex<Value>& value)
        {
            return log(re(value));
        }

    };
	
} // namespace logic
#endif
