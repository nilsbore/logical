#ifndef MATRIX_OPERATIONS_HPP
#define MATRIX_OPERATIONS_HPP

#include <iostream> // FOR DEBUG

namespace logic {
	
	template<typename Iter1, typename Iter2>
	struct assign {
		typedef Iter1 iter_type1;
		typedef Iter2 iter_type2;
		
		void operator() (Iter1 first, Iter2 second, unsigned end)
		{
			first.set(second);
			while(second != end) {
				*first = *second;
				first.set(++second);
			}
		}
	};
	
	template<typename Iter>
	struct assign_scalar {
		typedef Iter iter_type;
		typedef typename Iter::value_type value_type;
		
		void operator() (Iter i, unsigned end, const value_type& value)
		{
			while(i != end) {
				*i = value;
				++i;
			}
		}
	};
	
	template<typename Value>
	struct Add {
		
		void operator() (Value& first, const Value& second)
		{
			first += second;
		}
		
	};
	
	template<typename Iter>
	struct add_scalar {
		typedef Iter iter_type;
		typedef typename Iter::value_type value_type;
		
		void operator() (Iter i, unsigned end, const value_type& value)
		{
			while(i != end) {
				*i += value;
				++i;
			}
		}
	};
	
	template<typename Iter1, typename Iter2>
	struct subtract {
		typedef Iter1 iter_type1;
		typedef Iter2 iter_type2;
		
		void operator() (Iter1 first, Iter2 second, unsigned end)
		{
			first.set(second);
			while(second != end) {
				*first -= *second;
				first.set(++second);
			}
		}
	};
	
	template<typename Iter>
	struct subtract_scalar {
		typedef Iter iter_type;
		typedef typename Iter::value_type value_type;
		
		void operator() (Iter i, unsigned end, const value_type& value)
		{
			while(i != end) {
				*i -= value;
				++i;
			}
		}
	};
	
	template<typename Iter1, typename Iter2>
	struct multiply {
		typedef Iter1 iter_type1;
		typedef Iter2 iter_type2;
		
		void operator() (Iter1 first, Iter2 second, unsigned end)
		{
			second.set(first);
			while(first != end) {
				*first *= *second;
				second.set(++first);
			}
		}
	};
	
	template<typename Iter>
	struct multiply_scalar {
		typedef Iter iter_type;
		typedef typename Iter::value_type value_type;
		
		void operator() (Iter i, unsigned end, const value_type& value)
		{
			while(i != end) {
				*i *= value;
				++i;
			}
		}
	};
	
	template<typename Iter1, typename Iter2>
	struct divide {
		typedef Iter1 iter_type1;
		typedef Iter2 iter_type2;
		
		void operator() (Iter1 first, Iter2 second, unsigned end)
		{
			second.set(first);
			while(first != end) {
				*first /= *second;
				second.set(++first);
			}
		}
	};
	
	template<typename Iter>
	struct divide_scalar {
		typedef Iter iter_type;
		typedef typename Iter::value_type value_type;
		
		void operator() (Iter i, unsigned end, const value_type& value)
		{
			while(i != end) {
				*i /= value;
				++i;
			}
		}
	};
	
	template<typename Operator, typename Iter>
	struct apply {
		typedef Iter iter_type;
		
		void operator() (Iter i, unsigned end)
		{
			Operator op;
			while (i != end) {
				*i = op(*i);
				++i;
			}
		}
	};
	
	template<typename Operator, typename Iter1, typename Iter2>
	struct apply_and_store {
		typedef Iter1 iter_type1;
		typedef Iter2 iter_type2;
		
		void operator() (Iter1 first, Iter2 second, unsigned end)
		{
			Operator op;
			first.set(second);
			while (second != end) {
				*second = op(*first);
				first.set(++second);
			}
		}
	};
	
	template <typename Operator, typename Matrix1, typename Matrix2>
	struct Implementation {
		void operator() (row_iterator<Matrix1> first, row_iterator<Matrix2> second, unsigned end)
		{
			Operator op;
			first.set(second);
			while(second != end) {
				op(*first, *second);
				first.set(++second);
			}
		}
	};

    /* DEBUG
    template <typename Operator, typename Matrix1, typename Matrix2>
	struct Implementation<Operator, Matrix1, const typename Matrix2::sparse_type> {
		void operator() (row_iterator<Matrix1> first, row_iterator<const typename Matrix2::sparse_type> second, unsigned end)
		{
			std::cout << "Nu använde vi en sparse som andra!" << std::endl;
			Operator op;
			while(first != end) {
				op(*first, *second);
				++first;
			}
		}
	};
	
	template <typename Operator, typename Matrix1, typename Matrix2>
	struct Implementation<Operator, Matrix1, const typename Matrix2::scalar_type> {
		void operator() (row_iterator<Matrix1> first, row_iterator<const typename Matrix2::scalar_type> second, unsigned end)
		{
			std::cout << "Nu använde vi en sparse som andra!" << std::endl;
			Operator op;
			while(first != end) {
				op(*first, *second);
				++first;
			}
		}
	};
	
	template <typename Operator, typename Matrix1, typename Matrix2>
	struct Implementation<Operator, Matrix1, const typename Matrix2::dense_type> {
		void operator() (row_iterator<Matrix1> first, row_iterator<const typename Matrix2::dense_type> second, unsigned end)
		{
			std::cout << "Nu använde vi en sparse som andra!" << std::endl;
			Operator op;
			while(first != end) {
				op(*first, *second);
				++first;
			}
		}
    };*/ //DEBUG
	
	/*template <typename Matrix1, typename Matrix2>
	struct Implementation<Add<typename Matrix1::value_type>, Matrix1, Matrix2>
	{
		typedef typename Matrix1::value_type value_type;
		
		void operator() (row_iterator<Matrix1> first, row_iterator<Matrix2> second, unsigned end)
		{
			std::cout << "Nu använde vi addering med vanliga som andra!" << std::endl;
			Add<value_type> op;
			first.set(second);
			while(second != end) {
				op(*first, *second);
				first.set(++second);
			}
		}
		
	};*/
	
	/*template <typename Matrix1, typename Matrix2>
	struct Implementation<Add<typename Matrix1::value_type>, Matrix1, const typename Matrix2::sparse_type>
	{
		typedef typename Matrix1::value_type value_type;
		
		void operator() (row_iterator<Matrix1> first, row_iterator<const typename Matrix2::sparse_type> second, unsigned end)
		{
			std::cout << "Nu använde vi addering med sparse som andra!" << std::endl;
			Add<value_type> op;
			first.set(second);
			while(second != end) {
				op(*first, *second);
				first.set(++second);
			}
		}
		
	};
	
	template <typename Matrix1, typename Matrix2>
	struct Implementation<Add<typename Matrix1::value_type>, Matrix1, const typename Matrix2::scalar_type>
	{
		typedef typename Matrix1::value_type value_type;
		
		void operator() (row_iterator<Matrix1> first, row_iterator<const typename Matrix2::scalar_type> second, unsigned end)
		{
			std::cout << "Nu använde vi en skalär som andra!" << std::endl;
			Add<value_type> op();
			first.set(second);
			while(second != end) {
				op(*first, *second);
				first.set(++second);
			}
		}
		
	};*/
	
	template <typename Operator, typename Matrix1, typename Matrix2>
	void for_each_row(Matrix1& first, Matrix2& second)
	{
		typedef typename Matrix2::dense_type dense_type;
		typedef typename Matrix2::sparse_type sparse_type;
		typedef	typename Matrix2::scalar_type scalar_type;
		
		if (second.is_scalar()) { // kanske bara ta in värdet ist
			const scalar_type& sc = static_cast<const scalar_type&> (second);
			Implementation<Operator, Matrix1, const scalar_type> imp;
			for (unsigned y = 0; y < first.height(); ++y) {
				imp(first.begin_row(y), sc.begin_row(y), first.end_row());
			}
		}
		else if (second.is_dense()) {
			const dense_type& d = static_cast<const dense_type&> (second);
			Implementation<Operator, Matrix1, const dense_type> imp;
			for (unsigned y = 0; y < first.height(); ++y) {
				imp(first.begin_row(y), d.begin_row(y), first.end_row());
			}
		}
		else if (second.is_sparse()) {
			const sparse_type& s = static_cast<const sparse_type&> (second);
			Implementation<Operator, Matrix1, const sparse_type> imp;
			for (unsigned y = 0; y < first.height(); ++y) {
				imp(first.begin_row(y), s.begin_row(y), first.end_row());
			}
		}
		/*else {
		 for_each_row<add<row_iterator<Matrix1>, row_iterator<const Matrix2> > >(first, second);
		 }*/

	}
	
	template <typename Operator, typename Matrix>
	void for_each_row(typename Operator::iter_type::matrix_type& matrix,
					  const typename Operator::iter_type::value_type& value)
	{
		Operator op;
		for (unsigned y = 0; y < matrix.height(); ++y) {
			op(matrix.begin_row(y), matrix.end_row(), value);
		}
	}
	
	template <typename Operator>
	void for_each_row(typename Operator::iter_type::matrix_type& matrix)
	{
		Operator op;
		for (unsigned y = 0; y < matrix.height(); ++y) {
			op(matrix.begin_row(y), matrix.end_row());
		}
	}
	
	template <typename Matrix1, typename Matrix2>
	void add_rows(Matrix1& first, const Matrix2& second)
	{
		for_each_row<Add<typename Matrix1::value_type> >(first, second);
	}	
	
	template <typename Matrix1, typename Matrix2>
	void subtract_rows(Matrix1& first, const Matrix2& second)
	{
		
	}	
	
	template <typename Matrix1, typename Matrix2>
	void multiply_rows(Matrix1& first, const Matrix2& second)
	{
		
	}	
	
	template <typename Matrix1, typename Matrix2>
	void divide_rows(Matrix1& first, const Matrix2& second)
	{
		
	}
	
	template <typename Matrix1, typename Matrix2>
	void assign_rows(Matrix1& first, const Matrix2& second)
	{
		
	}
	
	template<typename Iter1, typename Iter2>
	typename Iter1::value_type scalar_product(Iter1 first, Iter2 second,
											  unsigned end)
	{
		
	}
	
	template <typename Matrix1, typename Matrix2, typename Matrix3>
	void product_detail(Matrix1& target, const Matrix2& first, const Matrix3& second)
	{
		
	}
	
	template <typename Matrix1, typename Matrix2>
	virtual_matrix<typename Matrix1::value_type>* matrix_product(const Matrix1& first,
																 const Matrix2& second)
	{
		/*typedef typename Matrix2::dense_type dense;
		 typedef typename Matrix2::sparse_type sparse;
		 
		 if (second.is_scalar()) {
		 Matrix1* rtn = new Matrix1(first);
		 (*rtn) *= second;
		 return rtn;
		 }
		 else if (second.is_sparse()) {
		 const sparse& s = static_cast<const sparse&>(second);
		 Matrix1* rtn = new Matrix1(first.height(), s.width());
		 product_detail(*rtn, first, s);
		 return rtn;
		 }
		 else if (second.is_dense()) {
		 const dense& d = static_cast<const dense&>(second);
		 dense* rtn = new dense(first.height(), d.width());
		 product_detail(*rtn, first, d);
		 return rtn;
		 }
		 else {
		 return NULL; // TODO!
		 }*/
		return NULL;
	}
	
	template <typename Operator, typename Matrix>
	Matrix* apply_function(Matrix& value)
	{
		return NULL;
	}
	
} // namespace logic
#endif
