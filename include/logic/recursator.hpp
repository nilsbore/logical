#ifndef RECURSATOR_HPP
#define RECURSATOR_HPP

#include "matrix_operations.hpp"
#include "../util/convenience.hpp"
#include "virtual_recursator.hpp"

namespace logic {

	template <typename Matrix>
	class recursator : public virtual_recursator<typename Matrix::value_type, util::is_const<Matrix>::value> {
	public:
		
		typedef typename Matrix::value_type value_type;
		typedef virtual_recursator<value_type, util::is_const<Matrix>::value> super;
        typedef typename super::dense_type dense_type;
        typedef typename super::sparse_type sparse_type;
        typedef typename super::scalar_type scalar_type;
		
	private:
		
		typedef recursator self;
		typedef typename util::if_<util::is_const<Matrix>,
				const Matrix&, Matrix&>::type ref_type;
		typedef typename util::if_<util::is_const<Matrix>,
				const value_type&, value_type&>::type access_type;
		
	public:
		
		access_type operator() (unsigned row, unsigned col)
		{
			return val(firstrow + row, firstcol + col);
		}
		
		const value_type& operator() (unsigned row, unsigned col) const
		{
			return ((const Matrix&)val)(firstrow + row, firstcol + col);
		}
		
		access_type operator[] (unsigned i)
		{
			if (height() == 1) {
				return (*this)(0, i);
			}
			return (*this)(i, 0);
		}
		
		const value_type& operator[] (unsigned i) const
		{
			if (height() == 1) {
				return (*this)(0, i);
			}
			return (*this)(i, 0);
		}
		
		unsigned height() const { return lastrow - firstrow + 1; }
		unsigned width() const { return lastcol - firstcol + 1; }
		
		/*super& operator= (const virtual_matrix<value_type>& other)
		{
			assign_rows(*this, other);
			return *this;
		}*/
		
		/*super& operator+= (const super& other)
		{
			add_rows(*this, other);
			return *this;
		}
		
		super& operator*= (const super& other)
		{
			multiply_rows(*this, other);
			return *this;
		}
		
		super& operator-= (const super& other)
		{
			subtract_rows(*this, other);
			return *this;
		}
		
		super& operator/= (const super& other)
		{
			divide_rows(*this, other);
			return *this;
		}
		
		virtual_matrix<value_type>* product(const super& other) const
		{
			return matrix_product(*this, other);
		}*/
		
        // Operators for non-constant recursator
		super& operator+= (const virtual_recursator<value_type, false>& other)
		{
			add_rows(*this, other);
			return *this;
		}
		
		super& operator*= (const virtual_recursator<value_type, false>& other)
		{
			multiply_rows(*this, other);
			return *this;
		}
		
		super& operator-= (const virtual_recursator<value_type, false>& other)
		{
			subtract_rows(*this, other);
			return *this;
		}
		
		super& operator/= (const virtual_recursator<value_type, false>& other)
		{
			divide_rows(*this, other);
			return *this;
		}
		
		virtual_matrix<value_type>* product(const virtual_recursator<value_type, false>& other) const
		{
			return matrix_product(*this, other);
		}
		
		// Operators for constant recursator
		super& operator+= (const virtual_recursator<value_type, true>& other)
		{
			add_rows(*this, other);
			return *this;
		}
		
		super& operator*= (const virtual_recursator<value_type, true>& other)
		{
			multiply_rows(*this, other);
			return *this;
		}
		
		super& operator-= (const virtual_recursator<value_type, true>& other)
		{
			subtract_rows(*this, other);
			return *this;
		}
		
		super& operator/= (const virtual_recursator<value_type, true>& other)
		{
			divide_rows(*this, other);
			return *this;
		}
		
		virtual_matrix<value_type>* product(const virtual_recursator<value_type, true>& other) const
		{
			return matrix_product(*this, other);
		}
		
		// Operators for matrices
        super& operator= (const virtual_matrix<value_type>& other)
        {
            assign_rows(*this, other);
            return *this;
        }

		super& operator+= (const virtual_matrix<value_type>& other)
		{
			add_rows(*this, other);
			return *this;
		}
		
		super& operator*= (const virtual_matrix<value_type>& other)
		{
			multiply_rows(*this, other);
			return *this;
		}
		
		super& operator-= (const virtual_matrix<value_type>& other)
		{
			subtract_rows(*this, other);
			return *this;
		}
		
		super& operator/= (const virtual_matrix<value_type>& other)
		{
			divide_rows(*this, other);
			return *this;
		}
		
		virtual_matrix<value_type>* product(const virtual_matrix<value_type>& other) const
		{
			return matrix_product(*this, other);
		}
						   
		row_iterator<self> begin_row(unsigned row)
		{
            row_iterator<self> rtn(val.begin_row(firstrow+row), firstcol, lastcol);
			return rtn;
		}
		
		row_iterator<const self> begin_row(unsigned row) const
		{
            row_iterator<const self> rtn(val.begin_row(firstrow+row), firstcol, lastcol);
			return rtn;
		}
		
		col_iterator<self> begin_col(unsigned col)
		{
            col_iterator<self> rtn(val.begin_col(firstcol+col), firstrow, lastrow);
			return rtn;
		}
						   
		col_iterator<const self> begin_col(unsigned col) const
		{
            col_iterator<const self> rtn(val.begin_col(firstcol+col), firstrow, lastrow);
			return rtn;
		}
						   
		unsigned end_row() const { return width(); }
		unsigned end_col() const { return height(); }
		
		recursator(ref_type val, unsigned firstrow, unsigned lastrow,
				   unsigned firstcol, unsigned lastcol) : super(val.get_type()), 
									val(val), firstrow(firstrow), lastrow(lastrow),
									firstcol(firstcol), lastcol(lastcol) {}
						   
	private:
		
		ref_type val;
		unsigned firstrow, lastrow, firstcol, lastcol;
	};
	
	template <typename Matrix>
	class row_iterator<recursator<Matrix> > : public row_iterator<Matrix> {
    protected:

        typedef row_iterator<Matrix> super;
        typedef typename super::base base;
        typedef typename base::base_const base_const;
        typedef typename base::base_mut base_mut;
        typedef row_iterator self;
        unsigned firstcol, lastcol;

    public:

        typedef recursator<Matrix> matrix_type;

        void set(const base_const& other) { super::explicit_set(firstcol + other.pos()); }
        void set(const base_mut& other) { super::explicit_set(firstcol + other.pos()); }
		void explicit_set(unsigned i) { super::explicit_set(firstcol + i); }
		unsigned pos() const { return super::pos() - firstcol; }
        //bool operator!= (unsigned end) const { return super::operator!=(firstcol + end); }
        bool operator!= (unsigned end) const { return end <= (lastcol - firstcol); }
        row_iterator(const super& other, unsigned firstcol, unsigned lastcol) : super(other),
            firstcol(firstcol), lastcol(lastcol)
		{
			while (super::pos() < firstcol) {
				++(*this);
			}
		}
	};
	
	template <typename Matrix> // TODO: subclass row_iterator<recursator> instead
	class row_iterator<const recursator<Matrix> > : public row_iterator<Matrix> {
    protected:

        typedef row_iterator<Matrix> super;
        typedef typename super::base base;
        typedef typename base::base_const base_const;
        typedef typename base::base_mut base_mut;
        typedef row_iterator self;
        unsigned firstcol, lastcol;

    public:

        typedef const recursator<Matrix> matrix_type;

        void set(const base_const& other) { super::explicit_set(firstcol + other.pos()); }
        void set(const base_mut& other) { super::explicit_set(firstcol + other.pos()); }
		void explicit_set(unsigned i) { super::explicit_set(firstcol + i); }
		unsigned pos() const { return super::pos() - firstcol; }
        //bool operator!= (unsigned end) const { return super::operator!=(firstcol + end); }
        bool operator!= (unsigned end) const { return end <= (lastcol - firstcol); }
        row_iterator(const super& other, unsigned firstcol, unsigned lastcol) : super(other),
        firstcol(firstcol), lastcol(lastcol)
		{
			while (super::pos() < firstcol) {
				++(*this);
			}
		}
	};
	
	template <typename Matrix>
	class col_iterator<recursator<Matrix> > : public col_iterator<Matrix> {
    protected:

        typedef col_iterator<Matrix> super;
        typedef typename super::base base;
        typedef typename base::base_const base_const;
        typedef typename base::base_mut base_mut;
		typedef col_iterator self;
        unsigned firstrow, lastrow;

	public:

        typedef recursator<Matrix> matrix_type;

        void set(const base_const& other) { super::explicit_set(firstrow + other.pos()); }
        void set(const base_mut& other) { super::explicit_set(firstrow + other.pos()); }
		void explicit_set(unsigned i) { super::explicit_set(firstrow + i); }
		unsigned pos() const { return super::pos() - firstrow; }
        //bool operator!= (unsigned end) const { return super::operator!=(firstrow + end); }
        bool operator!= (unsigned end) const { return end <= (lastrow - firstrow); }
        col_iterator(const super& other, unsigned firstrow, unsigned lastrow) : super(other),
            firstrow(firstrow), lastrow(lastrow)
		{
			while (super::pos() < firstrow) {
				++(*this);
			}
		}
	};
	
	template <typename Matrix> // TODO: subclass col_iterator<recursator> instead
	class col_iterator<const recursator<Matrix> > : public col_iterator<Matrix> {
    protected:

        typedef col_iterator<Matrix> super;
        typedef typename super::base base;
        typedef typename base::base_const base_const;
        typedef typename base::base_mut base_mut;
        typedef col_iterator self;
        unsigned firstrow, lastrow;

    public:

        typedef const recursator<Matrix> matrix_type;

        void set(const base_const& other) { super::explicit_set(firstrow + other.pos()); }
        void set(const base_mut& other) { super::explicit_set(firstrow + other.pos()); }
		void explicit_set(unsigned i) { super::explicit_set(firstrow + i); }
		unsigned pos() const { return super::pos() - firstrow; }
        //bool operator!= (unsigned end) const { return super::operator!=(firstrow + end); }
        bool operator!= (unsigned end) const { return end <= (lastrow - firstrow); }
        col_iterator(const super& other, unsigned firstrow, unsigned lastrow) : super(other),
        firstrow(firstrow), lastrow(lastrow)
		{
			while (super::pos() < firstrow) {
				++(*this);
			}
		}
	};
		
} // namespace logic
#endif
