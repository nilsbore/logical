#ifndef SCALAR_HPP
#define SCALAR_HPP

#include "virtual_matrix.hpp"
#include "recursator.hpp"

namespace logic {
	
	template <typename Value>
	class scalar : public virtual_matrix<Value> {
	private:
		
		typedef scalar self;
		
	public:
		
		typedef virtual_matrix<Value> super;
		typedef Value value_type;
		
		unsigned height() const { return 1; }
		unsigned width() const { return 1; }
		
		value_type& operator() (unsigned, unsigned) { return val; }
		const value_type& operator() (unsigned, unsigned) const { return val; }
		value_type& operator[] (unsigned) { return val; }
		const value_type& operator[] (unsigned) const { return val; }
		
		// Operators for matrices
		super& operator+= (const super& other)
		{
			val += other(0, 0);
			return *this;
		}
		
		super& operator-= (const super& other)
		{
			val -= other(0, 0);
			return *this;
		}
		
		super& operator*= (const super& other)
		{
			val *= other(0, 0);
			return *this;
		}
		
		super& operator/= (const super& other)
		{
			val /= other(0, 0);
			return *this;
		}
		
		super* product(const super& other) const
		{
			return new scalar<value_type>(val*other(0, 0));
		}
		
		// Operators for non-constant recursators
		super& operator+= (const virtual_recursator<value_type, false>& other)
		{
			val += other(0, 0);
			return *this;
		}
		
		super& operator-= (const virtual_recursator<value_type, false>& other)
		{
			val -= other(0, 0);
			return *this;
		}
		
		super& operator*= (const virtual_recursator<value_type, false>& other)
		{
			val *= other(0, 0);
			return *this;
		}
		
		super& operator/= (const virtual_recursator<value_type, false>& other)
		{
			val /= other(0, 0);
			return *this;
		}
		
		super* product(const virtual_recursator<value_type, false>& other) const
		{
			return new scalar<value_type>(val*other(0, 0));
		}
		
		// Operators for constant recursators
		super& operator+= (const virtual_recursator<value_type, true>& other)
		{
			val += other(0, 0);
			return *this;
		}
		
		super& operator-= (const virtual_recursator<value_type, true>& other)
		{
			val -= other(0, 0);
			return *this;
		}
		
		super& operator*= (const virtual_recursator<value_type, true>& other)
		{
			val *= other(0, 0);
			return *this;
		}
		
		super& operator/= (const virtual_recursator<value_type, true>& other)
		{
			val /= other(0, 0);
			return *this;
		}
		
		super* product(const virtual_recursator<value_type, true>& other) const
		{
			return new scalar<value_type>(val*other(0, 0));
		}
		
		self& operator= (const self& other)
		{
			val = other.val;
			return *this;
		}
		
		/*super& operator= (const super& other)
		{
			val = other(0, 0);
			return *this;
		}*/
		
		void transp() {}
		
		typename super::recursator_type* new_recursator(unsigned firstrow, unsigned lastrow,
														unsigned firstcol, unsigned lastcol)
		{
			return new recursator<self>(*this, 0, 0, 0, 0);
		}
		
		// These methods are for being able to define a scalar recursator
		row_iterator<self> begin_row(unsigned row)
		{
			return row_iterator<self>(*this, 0);
		}
		
		row_iterator<const self> begin_row(unsigned row) const
		{
			return row_iterator<const self>(*this, 0);
		}
		
		col_iterator<self> begin_col(unsigned col)
		{
			return col_iterator<self>(*this, 0);
		}
		
		col_iterator<const self> begin_col(unsigned col) const
		{
			return col_iterator<const self>(*this, 0);
		}
		
		scalar(const self& other) : super(super::SCALAR), val(other.val) {}
		scalar(value_type val) : super(super::SCALAR), val(val) {}
		
	private:
		
		value_type val;
	};

} // namespace logic
#endif