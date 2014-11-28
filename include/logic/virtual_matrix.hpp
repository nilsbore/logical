#ifndef VIRTUAL_MATRIX_HPP
#define VIRTUAL_MATRIX_HPP

#include "../util/convenience.hpp"
#include "virtual_recursator.hpp"

namespace logic {
	
	template<typename Matrix>
	class row_iterator {
	public:
		
		typedef typename Matrix::value_type value_type;
		typedef Matrix matrix_type;
		
	private:
		
		typedef row_iterator self;
		typedef typename util::if_<util::is_const<Matrix>,
				const Matrix&, Matrix&>::type ref_type;
		typedef typename util::if_<util::is_const<Matrix>,
				const value_type&, value_type&>::type access_type;
		
	public:
		
		access_type operator* ()
		{
			return val(row, current);
		}
		
		const value_type& operator* () const
		{
			return val(row, current);
		}
		
		self& operator++ ()
		{
			current++;
			return *this;
		}
		
		bool operator!= (unsigned end) const {
			return current < end;
		}
		
		unsigned pos() const
		{
			if (current < val.width()) {
				return current;
			}
			return val.width();
		}
		
        void explicit_set(unsigned i) { current = i; }
		
		template <typename Iterator>
		void set(const Iterator& other) { current = other.pos(); }
		
		row_iterator(ref_type val, unsigned row) : val(val), row(row),
											current(0) {}	
		row_iterator(const self& other) : val(other.val), row(other.row),
											current(other.current) {}
	private:
		
		ref_type val;
		unsigned row, current;
	};
	
	template<typename Matrix>
	class col_iterator {		
	public:
		
		typedef typename Matrix::value_type value_type;
		
	private:
		
		typedef col_iterator self;
		typedef typename util::if_<util::is_const<Matrix>,
				const Matrix&, Matrix&>::type ref_type;
		typedef typename util::if_<util::is_const<Matrix>,
				const value_type&, value_type&>::type access_type;
		
	public:
		
		access_type operator* ()
		{
			return val(current, col);
		}
		
		const value_type& operator* () const
		{
			return val(current, col);
		}
		
		self& operator++ ()
		{
			current++;
			return *this;
		}
		
		bool operator!= (unsigned end) const {
			return current < end;
		}
		
		unsigned pos() const
		{
			if (pos < val.height()) {
				return pos;
			}
			return val.height();
		}
		
        void explicit_set(unsigned i) { current = i; }
		
		template<typename Iterator>
		void set(const Iterator& other) { current = other.pos(); }
		
		col_iterator(ref_type val, unsigned col) : val(val), col(col),
												current(0) {}
		col_iterator(const self& other) : val(other.val), col(other.col),
												current(other.current) {}
	private:
		
		ref_type val;
		unsigned col, current;
	};
	
	template <typename Value>
	class virtual_matrix {
	private:
		
		typedef virtual_matrix self;
		
	protected:
		
		typedef virtual_recursator<Value, util::is_const<self>::value> recursator_type;
		// may be useful to have public?
		
	public:
		
		typedef Value value_type;
		typedef dense_matrix<value_type> dense_type;
		typedef sparse_matrix<value_type> sparse_type;
		typedef scalar<value_type> scalar_type;
        typedef string_matrix<value_type> string_type;
		
        enum matrix_type {DENSE, SPARSE, SCALAR, STRING, NONE} type;

        value_type dummy;
		
        virtual value_type& operator() (unsigned, unsigned) { return dummy; }
        virtual const value_type& operator() (unsigned, unsigned) const { return 0; }
        virtual value_type& operator[] (unsigned) { return dummy; }
        virtual const value_type& operator[] (unsigned) const { return 0; }
        virtual unsigned height() const { return 0; }
        virtual unsigned width() const {return 0; }
		unsigned length() const
		{
			if (height() == 1) {
				return width();
			}
			return height();
		}
        bool is_empty() const { return height() == 0 || width() == 0; }
        operator bool() const { return (*this)(0, 0) != value_type(0); }
		
        virtual self& operator+= (const self&) { return *this; }
        virtual self& operator*= (const self&) { return *this; }
        //virtual self& operator= (const self&) {}
        virtual self& operator-= (const self&) { return *this; }
        virtual self& operator/= (const self&) { return *this; }
        virtual self* product(const self&) const { return new self(NONE); }
        /*virtual self& operator^= (self&) {} {}*/
		
        virtual self& operator+= (const virtual_recursator<value_type, false>&) { return *this; }
        virtual self& operator*= (const virtual_recursator<value_type, false>&) { return *this; }
        virtual self& operator-= (const virtual_recursator<value_type, false>&) { return *this; }
        virtual self& operator/= (const virtual_recursator<value_type, false>&) { return *this; }
        virtual self* product(const virtual_recursator<value_type, false>&) const { return new self(NONE); }
		
        virtual self& operator+= (const virtual_recursator<value_type, true>&) { return *this; }
        virtual self& operator*= (const virtual_recursator<value_type, true>&) { return *this; }
        virtual self& operator-= (const virtual_recursator<value_type, true>&) { return *this; }
        virtual self& operator/= (const virtual_recursator<value_type, true>&) { return *this; }
        virtual self* product(const virtual_recursator<value_type, true>&) const { return new self(NONE); }
		
        virtual void transp() {}
		
		matrix_type get_type() const { return type; }
		bool is_dense() const { return type == DENSE || type == STRING; }
		bool is_scalar() const { return height() == 1 && width() == 1; }
		bool is_sparse() const { return type == SPARSE; }
		bool is_string() const { return type == STRING; }
		bool is_vector() const { return height() == 1 || width() == 1; }
        void set_string(bool is) { if (is) type = STRING; }
		
        virtual recursator_type* new_recursator(unsigned, unsigned, unsigned, unsigned) {}
		
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
		
        virtual self* clone() const { return new self(NONE); }
        virtual self* instantiate(size_t rows, size_t cols) const { return new self(NONE); }
		virtual_matrix(matrix_type type) : type(type) {}
        virtual_matrix() : type(NONE) {}
		virtual ~virtual_matrix() {}
	};
	
	template <typename OutStream, typename Value>
	OutStream& operator<< (OutStream& s, const virtual_matrix<Value>& m)
	{
		typedef virtual_matrix<Value> matrix;
        typedef typename matrix::string_type string_type;
        if (m.get_type() == matrix::STRING) {
            s << static_cast<const string_type&>(m);
            s << "STRING" << '\n';
            return s;
        }
        s << "NOT STRING" << '\n';
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
