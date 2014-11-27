#ifndef DENSE_MATRIX_HPP
#define DENSE_MATRIX_HPP

#include "virtual_matrix.hpp"
#include "matrix_operations.hpp"
#include "recursator.hpp"

namespace logic {
	
	template <typename Value>
	class dense_matrix : public virtual_matrix<Value> {
	private:
		
		typedef dense_matrix self;
		
	public:
		
		typedef virtual_matrix<Value> super;
		typedef Value value_type;
		
		unsigned height() const { return rows; }
		unsigned width() const { return cols; }
		
		value_type& operator() (unsigned y, unsigned x)
		{
			return vals[y][x];
		}
		
		const value_type& operator() (unsigned y, unsigned x) const
		{
			return vals[y][x];
		}
		
		value_type& operator[] (unsigned i)
		{
			if (rows == 1) {
				return vals[0][i];
			}
			return vals[i][0];
		}
		
		const value_type& operator[] (unsigned i) const
		{
			if (rows == 1) {
				return vals[0][i];
			}
			return vals[i][0];
		}
		
		// Operators for matrices
		super& operator+= (const super& other)
		{
			add_rows(*this, other);
			return *this;
		}
		
		super& operator-= (const super& other)
		{
			subtract_rows(*this, other);
			return *this;
		}
		
		super& operator*= (const super& other)
		{
			multiply_rows(*this, other);
			return *this;
		}
		
		super& operator/= (const super& other)
		{
			divide_rows(*this, other);
			return *this;
		}
		
		super* product(const super& other) const
		{
			return matrix_product(*this, other);
		}
		
		// Operators for non-constant recursators
		super& operator+= (const virtual_recursator<value_type, false>& other)
		{
			add_rows(*this, other);
			return *this;
		}
		
		super& operator-= (const virtual_recursator<value_type, false>& other)
		{
			subtract_rows(*this, other);
			return *this;
		}
		
		super& operator*= (const virtual_recursator<value_type, false>& other)
		{
			multiply_rows(*this, other);
			return *this;
		}
		
		super& operator/= (const virtual_recursator<value_type, false>& other)
		{
			divide_rows(*this, other);
			return *this;
		}
		
		super* product(const virtual_recursator<value_type, false>& other) const
		{
			return matrix_product(*this, other);
		}
		
		// Operators for constant recursators
		super& operator+= (const virtual_recursator<value_type, true>& other)
		{
			add_rows(*this, other);
			return *this;
		}
		
		super& operator-= (const virtual_recursator<value_type, true>& other)
		{
			subtract_rows(*this, other);
			return *this;
		}
		
		super& operator*= (const virtual_recursator<value_type, true>& other)
		{
			multiply_rows(*this, other);
			return *this;
		}
		
		super& operator/= (const virtual_recursator<value_type, true>& other)
		{
			divide_rows(*this, other);
			return *this;
		}
		
		super* product(const virtual_recursator<value_type, true>& other) const
		{
			return matrix_product(*this, other);
		}
		
		self& operator= (const self& other)
		{
			unsigned a = 2;
			return *this;
		}
		
		/*super& operator= (const super& other)
		{
			for (unsigned y = 0; y < rows; ++y) {
				for (unsigned x = 0; x < cols; ++x) {
					(*this)(y, x) = other(y, x);
				}
			}
			return *this;
		}*/
		
		void transp()
		{
			self newval(cols, rows);
			for (unsigned y = 0; y < rows; ++y) {
				for (unsigned x = 0; x < cols; ++x) {
					newval(x, y) = (*this)(y, x);
				}
			}
			*this = newval; // titta lite mer på detta
		}
		
		typename super::recursator_type* new_recursator(unsigned firstrow, unsigned lastrow,
														unsigned firstcol, unsigned lastcol)
		{
			return new recursator<self>(*this, firstrow, lastrow, firstcol, lastcol);
		}
		
		row_iterator<self> begin_row(unsigned row)
		{
			return row_iterator<self>(*this, row);
		}
		
		row_iterator<const self> begin_row(unsigned row) const
		{
			return row_iterator<const self>(*this, row);
		}
		
		col_iterator<self> begin_col(unsigned col)
		{
			return col_iterator<self>(*this, col);
		}
		
		col_iterator<const self> begin_col(unsigned col) const
		{
			return col_iterator<const self>(*this, col);
		}
		
		unsigned end_row() const { return cols; }
		unsigned end_col() const { return rows; }
		
		dense_matrix(const self& other) : super(super::DENSE),
										  rows(other.rows), cols(other.cols)
		{
			vals = new value_type*[rows];
			for (unsigned y = 0; y < rows; ++y) {
				vals[y] = new value_type[cols];
				for (unsigned x = 0; x < cols; ++x) {
					(*this)(y, x) = other(y, x);
				}
			}
		}
		
		dense_matrix(unsigned rows, unsigned cols) : super(super::DENSE),
													 rows(rows), cols(cols) 
		{
			vals = new value_type*[rows];
			for (unsigned y = 0; y < rows; ++y) {
				vals[y] = new value_type[cols];
			}
		}
		
		dense_matrix(unsigned rows, unsigned cols, typename super::matrix_type type) :
													super(type), rows(rows), cols(cols) 
		{
			vals = new value_type*[rows];
			for (unsigned y = 0; y < rows; ++y) {
				vals[y] = new value_type[cols];
			}
		}
		
		~dense_matrix()
		{
			for (unsigned i = 0; i < rows; ++i) {
				delete[] vals[i];
			}
			delete[] vals;			
		}
		
	private:
		
		unsigned rows, cols;
		value_type** vals;
	};

} // namespace logic
#endif