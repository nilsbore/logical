#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "../util/string.hpp"

namespace logic {
	
	template<typename Matrix>
	Matrix get_constant(const util::string& name)
	{
		Matrix rtn;
		switch (name) {
			case "e":
				rtn = Matrix::real_scalar(2.71828183);
				break;
            case "pi":
				rtn = Matrix::real_scalar(3.14159265);
				break;
            case "author":
				rtn = Matrix(util::string("Nils Bore"));
				break;
		}
		return rtn;
	}
	
	// leaving space for functions
	
} // namespace logic
#endif
