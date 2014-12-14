#ifndef MATRIX_FUNCTIONS_HPP
#define MATRIX_FUNCTIONS_HPP

#include "../util/info_auto_ptr.hpp"
#include "../util/list.hpp"
#include "matrix_operations.hpp"
#include "functors.hpp"
#include "matrix_factory.hpp"

namespace logic {
	
	template<typename Matrix>
	util::info_auto_ptr<Matrix, util::string> linspace(const Matrix& first,
													   const Matrix& second)
	{
        typedef typename Matrix::dense_type dense_type;

        if (!first.is_scalar() || !second.is_scalar()) return "Linspace boundaries are not scalar.";
		
		int a = first(0, 0);
		int b = second(0, 0);
		
		unsigned length = abs(b - a) + 1;
		int step = b > a? 1 : -1;
		
        Matrix* rtn = new dense_type(1, length);
		for (unsigned i = 0; i < length; i++) {
            (*rtn)(0, i) = a;
			a += step;
		}
		
		return rtn;
	}
	
	template<typename Matrix>
	util::info_auto_ptr<Matrix, util::string> linspace(const Matrix& first,
													   const Matrix& second,
													   const Matrix& third)
	{
		typedef typename Matrix::value_type value_type;
        typedef typename Matrix::dense_type dense_type;
		
        if (!second.is_scalar()) return "Linspace step is not scalar.";
        if (first.width() != 1 || third.width() != 1) return "Linspace boundaries are not column vectors.";
		if (first.height() != third.height()) return "Linspace boundaries heights do not agree.";
		
		unsigned length = second(0, 0);
		
        Matrix* rtn = new dense_type(first.height(), length); // dense
		for (unsigned i = 0; i < first.height(); ++i) {
            value_type a = first(i, 0);
			value_type b = third(i, 0);
			value_type step = (b - a) / value_type(length - 1);
			for (unsigned j = 0; j < length; ++j) {
                (*rtn)(i, j) = a;
				a += step;
			}
		}
		
		return rtn;
	}
	
    // TODO: Use iterators or recursators for this instead
	template<typename Matrix>
	util::info_auto_ptr<Matrix, util::string> index(const Matrix& value, const Matrix& pos)
	{
		if (!value.is_vector()) return "One argument index doesn't operate on vector.";
		
		unsigned rows = pos.height();
		unsigned cols = pos.width();
		
        Matrix* rtn = value.instantiate(rows, cols);
        rtn->set_string(value.is_string());

		for (unsigned y = 0; y < rows; ++y) {
			for (unsigned x = 0; x < cols; ++x) {
                (*rtn)(y, x) = value[(unsigned)pos(y, x)];
			}
		}
		
		return rtn;
	}
	
    // TODO: Use iterators or recursators for this instead
	template<typename Matrix>
	util::info_auto_ptr<Matrix, util::string> index(const Matrix& value, const Matrix& row,
													const Matrix& col)
	{
		if(!row.is_vector() || !col.is_vector()) return "Index arguments are not vectors.";
		
		unsigned rows = row.length();
		unsigned cols = col.length();
		
        Matrix* rtn = value.instantiate(rows, cols);
		
        rtn->set_string(value.is_string());
		
		for (unsigned y = 0; y < rows; ++y) {
			for (unsigned x = 0; x < cols; ++x) {
                (*rtn)(y, x) = value((unsigned)row[y], (unsigned)col[x]);
			}
		}
		
		return rtn;
	}
	
    // TODO: Use iterators or recursators for this instead
	template<typename Matrix>
	bool assign_index(Matrix& var, const Matrix& pos, const Matrix& value)
	{
		if(!var.is_vector()) {
			return false;
		}
		
		unsigned rows = pos.height();
		unsigned cols = pos.width();
		
		if (value.height() != rows || value.width() != cols) {
			return false;
		}
		
		for (unsigned y = 0; y < rows; ++y) {
			for (unsigned x = 0; x < cols; ++x) {
				var[pos(y, x)] = value(y, x);
			}
		}
		
		return true;
	}
	
    // TODO: Use iterators or recursators for this instead
	template<typename Matrix>
	bool assign_index(Matrix& var, const Matrix& row, const Matrix& col, const Matrix& value)
	{
		if(!row.is_vector() || !col.is_vector()) {
			return false;
		}
		
		unsigned rows = row.length();
		unsigned cols = col.length();
		
		if (value.height() != rows || value.width() != cols) {
			return false;
		}
		
		for (unsigned y = 0; y < rows; ++y) {
			for (unsigned x = 0; x < cols; ++x) {
				var(row[y], col[x]) = value(y, x);
			}
		}
		
		return true;
	}
	
	template<typename Matrix>
	util::info_auto_ptr<Matrix, util::string> base_functions(const util::string& name,
															 util::list<Matrix>& args)
	{
		typedef typename Matrix::value_type value_type;
		
		Matrix* rtn;
		if (name == "sin") {
			if (args.length() != 1) return "Sin only takes one input argument.";
            rtn = apply_function<Sin<value_type> >(*args);
		}
		else if (name == "cos") {
			if (args.length() != 1) return "Cos only takes one input argument.";
            rtn = apply_function<Cos<value_type> >(*args);
		}
		else if (name == "exp") {
			if (args.length() != 1) return "Exp only takes one input argument.";
            rtn = apply_function<Exp<value_type> >(*args);
		}
		else if (name == "log") {
			if (args.length() != 1) return "Log only takes one input argument.";
            rtn = apply_function<Log<value_type> >(*args);
		}
        else if (name == "eye") {
            return eye<value_type>((*args)(0, 0));
        }
        else if (name == "diag") {
            return diag<value_type>(*args);
        }
		else {
			return "Function not found.";
		}
		if (rtn == NULL) {
			return args.release();
		}
		return rtn;
	}

} // namespace logic
#endif
