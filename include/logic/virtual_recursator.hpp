#ifndef VIRTUAL_RECURSATOR_HPP
#define VIRTUAL_RECURSATOR_HPP

#include "../util/convenience.hpp"
#include "iterator.hpp"

namespace logic {
	
	template <typename Matrix> class recursator;
	template <typename Value> class virtual_matrix;
	template <typename Value> class sparse_matrix;
	template <typename Value> class dense_matrix;
    template <typename Value> class string_matrix;
	template <typename Value> class scalar;

	template <typename Value, bool IsConst = false>
	class virtual_recursator {
	private:
		
		typedef typename util::if_c<IsConst, const Value&, Value&>::type access_type;
		typedef virtual_recursator self;
		typedef virtual_matrix<Value> vmatrix;
		
	public:
		
		typedef Value value_type;
		typedef typename vmatrix::matrix_type matrix_type;
        typedef virtual_recursator virtual_type;
		typedef recursator<typename util::if_c<IsConst, const dense_matrix<value_type>,
				dense_matrix<value_type> >::type> dense_type;
		typedef recursator<typename util::if_c<IsConst, const sparse_matrix<value_type>,
				sparse_matrix<value_type> >::type> sparse_type;
		typedef recursator<typename util::if_c<IsConst, const scalar<value_type>,
				scalar<value_type> >::type> scalar_type;
		
		virtual unsigned height() const = 0;
		virtual unsigned width() const = 0;
		unsigned length() const
		{
			if (height() == 1) {
				return width();
			}
			return height();
		}
		
		matrix_type get_type() const { return type; }
		bool is_dense() const { return type == vmatrix::DENSE || type == vmatrix::STRING; }
		bool is_scalar() const { return height() == 1 && width() == 1; }
		bool is_sparse() const { return type == vmatrix::SPARSE; }
		bool is_string() const { return type == vmatrix::STRING; }
		bool is_vector() const { return height() == 1 || width() == 1; }
		
		virtual access_type operator() (unsigned row, unsigned col) = 0;
		virtual const value_type& operator() (unsigned row, unsigned col) const = 0;		
		virtual access_type operator[] (unsigned i) = 0;		
		virtual const value_type& operator[] (unsigned i) const = 0;
		
		//virtual self& operator= (const virtual_matrix<value_type>&) = 0;
		/*virtual self& operator+= (const self&) = 0;
		virtual self& operator*= (const self&) = 0;
		virtual self& operator-= (const self&) = 0;
		virtual self& operator/= (const self&) = 0;
		virtual virtual_matrix<value_type>* product(const self& other) const = 0;*/
		
        virtual self& operator= (const virtual_matrix<value_type>&) = 0;
		virtual self& operator+= (const virtual_matrix<value_type>&) = 0;
		virtual self& operator*= (const virtual_matrix<value_type>&) = 0;
		virtual self& operator-= (const virtual_matrix<value_type>&) = 0;
		virtual self& operator/= (const virtual_matrix<value_type>&) = 0;
		virtual virtual_matrix<value_type>* product(const virtual_matrix<value_type>&) const = 0;
		
		virtual self& operator+= (const virtual_recursator<value_type, false>&) = 0;
		virtual self& operator*= (const virtual_recursator<value_type, false>&) = 0;
		virtual self& operator-= (const virtual_recursator<value_type, false>&) = 0;
		virtual self& operator/= (const virtual_recursator<value_type, false>&) = 0;
		virtual virtual_matrix<value_type>* product(const virtual_recursator<value_type, false>&) const = 0;
		
		virtual self& operator+= (const virtual_recursator<value_type, true>&) = 0;
		virtual self& operator*= (const virtual_recursator<value_type, true>&) = 0;
		virtual self& operator-= (const virtual_recursator<value_type, true>&) = 0;
		virtual self& operator/= (const virtual_recursator<value_type, true>&) = 0;
		virtual virtual_matrix<value_type>* product(const virtual_recursator<value_type, true>&) const = 0;

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
		
		virtual_recursator(matrix_type type) : type(type) {}
		virtual ~virtual_recursator() {}
		
	private:
		
		matrix_type type;
	};
	
	template <typename OutStream, typename Value, bool IsConst>
	OutStream& operator<< (OutStream& s, const virtual_recursator<Value, IsConst>& r)
	{
		for (unsigned y = 0; y < r.height(); ++y) {
			for (unsigned x = 0; x < r.width(); ++x) {
				s << r(y, x) << ' ';
			}
			s << '\n';
		}
		return s;
	}
	
} // namespace logic
#endif
