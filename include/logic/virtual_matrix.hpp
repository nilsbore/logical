#ifndef VIRTUAL_MATRIX_HPP
#define VIRTUAL_MATRIX_HPP

#include "../util/convenience.hpp"
#include "virtual_recursator.hpp"
#include "iterator.hpp"

namespace logic {
	
	template <typename Value>
	class virtual_matrix {
	private:
		
		typedef virtual_matrix self;
		
	public:
		
		typedef Value value_type;
        typedef virtual_matrix virtual_type;
		typedef dense_matrix<value_type> dense_type;
		typedef sparse_matrix<value_type> sparse_type;
		typedef scalar<value_type> scalar_type;
        typedef string_matrix<value_type> string_type;
        typedef virtual_recursator<Value, util::is_const<self>::value> recursator_type;
		
        enum matrix_type {DENSE, SPARSE, SCALAR, STRING, NONE} type;

        //value_type dummy;
		
        virtual value_type& operator() (unsigned, unsigned) = 0; //{ return dummy; }
        virtual const value_type& operator() (unsigned, unsigned) const = 0; // { return 0; }
        virtual value_type& operator[] (unsigned) = 0; // { return dummy; }
        virtual const value_type& operator[] (unsigned) const = 0; // { return 0; }
        virtual unsigned height() const = 0; // { return 0; }
        virtual unsigned width() const = 0; // {return 0; }
		unsigned length() const
		{
			if (height() == 1) {
				return width();
			}
			return height();
		}
        bool is_empty() const { return height() == 0 || width() == 0; }
        operator bool() const { return (*this)(0, 0) != value_type(0); }
		
        virtual self& operator+= (const self&) = 0; // { return *this; }
        virtual self& operator*= (const self&) = 0; // { return *this; }
        //virtual self& operator= (const self&) {}
        virtual self& operator-= (const self&) = 0; // { return *this; }
        virtual self& operator/= (const self&) = 0; // { return *this; }
        virtual self* product(const self&) const = 0; // { return new self(NONE); }
        /*virtual self& operator^= (self&) {} {}*/
		
        virtual self& operator+= (const virtual_recursator<value_type, false>&) = 0; // { return *this; }
        virtual self& operator*= (const virtual_recursator<value_type, false>&) = 0; // { return *this; }
        virtual self& operator-= (const virtual_recursator<value_type, false>&) = 0; // { return *this; }
        virtual self& operator/= (const virtual_recursator<value_type, false>&) = 0; // { return *this; }
        virtual self* product(const virtual_recursator<value_type, false>&) const = 0; // { return new self(NONE); }
		
        virtual self& operator+= (const virtual_recursator<value_type, true>&) = 0; // { return *this; }
        virtual self& operator*= (const virtual_recursator<value_type, true>&) = 0; // { return *this; }
        virtual self& operator-= (const virtual_recursator<value_type, true>&) = 0; // { return *this; }
        virtual self& operator/= (const virtual_recursator<value_type, true>&) = 0; // { return *this; }
        virtual self* product(const virtual_recursator<value_type, true>&) const = 0; // { return new self(NONE); }
		
        virtual void transp() = 0; // {}
		
		matrix_type get_type() const { return type; }
		bool is_dense() const { return type == DENSE || type == STRING; }
		bool is_scalar() const { return height() == 1 && width() == 1; }
        bool is_true_scalar() const { return type == SCALAR; }
		bool is_sparse() const { return type == SPARSE; }
		bool is_string() const { return type == STRING; }
		bool is_vector() const { return height() == 1 || width() == 1; }
        void set_string(bool is) { if (is) type = STRING; }
		
        virtual recursator_type* new_recursator(unsigned, unsigned, unsigned, unsigned) = 0; // {}
		
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
		
		unsigned end_row() const { return width(); }
		unsigned end_col() const { return height(); }
		
        virtual self* clone() const = 0; // { return new self(NONE); }
        virtual self* instantiate(size_t rows, size_t cols) const = 0; // { return new self(NONE); }
		virtual_matrix(matrix_type type) : type(type) {}
        //virtual_matrix() : type(NONE) {}
		virtual ~virtual_matrix() {}
	};
	
	template <typename OutStream, typename Value>
	OutStream& operator<< (OutStream& s, const virtual_matrix<Value>& m)
	{
		typedef virtual_matrix<Value> matrix;
        typedef typename matrix::string_type string_type;
        if (m.get_type() == matrix::STRING) {
            s << static_cast<const string_type&>(m);
            return s;
        }
		for (unsigned y = 0; y < m.height(); ++y) {
			for (unsigned x = 0; x < m.width(); ++x) {
				s << m(y, x) << ' ';
			}
			s << '\n';
		}
		return s;
	}

} // namespace logic
#endif
