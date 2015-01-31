#ifndef MATRIX_OPERATIONS_HPP
#define MATRIX_OPERATIONS_HPP

#include "scalar.hpp"
#include "dense_matrix.hpp"

#include <iostream> // DEBUG

namespace logic {

    template <typename> class sparse_matrix;

    template<typename Matrix1, typename Matrix2>
    struct Add {
        typedef row_iterator<Matrix1> iter_type1;
        typedef row_iterator<Matrix2> iter_type2;

        void operator() (Matrix1& first, Matrix2& second)
        {
            std::cout << "Using my fancy new Add function" << std::endl;
            for (unsigned y = 0; y < first.height(); ++y) {
                iter_type1 it1 = first.begin_row(y);
                iter_type2 it2 = second.begin_row(y);
                it1.set(it2);
                while (it2 != first.end_row()) {
                    *it1 += *it2;
                    it1.set(++it2);
                }
            }
        }
    };

    template<typename Matrix1>
    struct Add<Matrix1, const sparse_matrix<typename Matrix1::value_type> > {
        typedef const sparse_matrix<typename Matrix1::value_type> sparse_type;
        typedef row_iterator<Matrix1> iter_type1;
        typedef row_iterator<sparse_type> iter_type2;

        void operator() (Matrix1& first, sparse_type& second)
        {
            std::cout << "Using my fancy new Add function with sparse matrices" << std::endl;
            for (unsigned y = 0; y < first.height(); ++y) {
                iter_type1 it1 = first.begin_row(y);
                iter_type2 it2 = second.begin_row(y);
                it1.set(it2);
                while (it2 != first.end_row()) {
                    *it1 += *it2;
                    it1.set(++it2);
                }
            }
        }
    };

    template<typename Matrix1>
    struct Add<Matrix1, const scalar<typename Matrix1::value_type> > {
        typedef const scalar<typename Matrix1::value_type> scalar_type;
        typedef row_iterator<Matrix1> iter_type;

        void operator() (Matrix1& first, scalar_type& second)
        {
            std::cout << "Using my fancy new Add function with a scalar" << std::endl;
            for (unsigned y = 0; y < first.height(); ++y) {
                iter_type it = first.begin_row(y);
                while (it != first.end_row()) {
                    *it += second(0, 0);
                    ++it;
                }
            }
        }
    };

    template <>
    struct Add<scalar<double>, const scalar<double> > {

        void operator() (scalar<double>& first, const scalar<double>& second)
        {
            std::cout << "Using my fancy new Add function with only scalars" << std::endl;
            first(0, 0) += second(0, 0);
        }
    };

    template<typename Matrix1, typename Matrix2>
    struct Subtract {
        typedef row_iterator<Matrix1> iter_type1;
        typedef row_iterator<Matrix2> iter_type2;

        void operator() (Matrix1& first, Matrix2& second)
        {
            std::cout << "Using my fancy new Subtract function" << std::endl;
            for (unsigned y = 0; y < first.height(); ++y) {
                iter_type1 it1 = first.begin_row(y);
                iter_type2 it2 = second.begin_row(y);
                it1.set(it2);
                while (it2 != first.end_row()) {
                    *it1 -= *it2;
                    it1.set(++it2);
                }
            }
        }
    };

    template<typename Matrix1>
    struct Subtract<Matrix1, const scalar<typename Matrix1::value_type> > {
        typedef const scalar<typename Matrix1::value_type> scalar_type;
        typedef row_iterator<Matrix1> iter_type;

        void operator() (Matrix1& first, scalar_type& second)
        {
            std::cout << "Using my fancy new Subtract function with a scalar" << std::endl;
            for (unsigned y = 0; y < first.height(); ++y) {
                iter_type it = first.begin_row(y);
                while (it != first.end_row()) {
                    *it -= second(0, 0);
                    ++it;
                }
            }
        }
    };

    template <>
    struct Subtract<scalar<double>, const scalar<double> > {

        void operator() (scalar<double>& first, const scalar<double>& second)
        {
            std::cout << "Using my fancy new Subtract function with only scalars" << std::endl;
            first(0, 0) -= second(0, 0);
        }
    };

    template<typename Matrix1, typename Matrix2>
    struct Assign {
        typedef row_iterator<Matrix1> iter_type1;
        typedef row_iterator<Matrix2> iter_type2;

        void operator() (Matrix1& first, Matrix2& second)
        {
            std::cout << "Using my fancy new Assign function" << std::endl;
            for (unsigned y = 0; y < first.height(); ++y) {
                iter_type1 it1 = first.begin_row(y);
                iter_type2 it2 = second.begin_row(y);
                it1.set(it2);
                while (it2 != first.end_row()) {
                    *it1 = *it2;
                    it1.set(++it2);
                }
            }
        }
    };

    /*template<typename Matrix1>
    struct Assign<Matrix1, const scalar<double> > {
        typedef row_iterator<Matrix1> iter_type;

        void operator() (Matrix1& first, const scalar<double>& second)
        {
            std::cout << "Using my fancy new Assign function with a scalar" << std::endl;
            for (unsigned y = 0; y < first.height(); ++y) {
                iter_type it = first.begin_row(y);
                while(it != first.end_row()) {
                    *it = second(0, 0);
                    ++it;
                }
            }
        }
    };*/

    template <>
    struct Assign<recursator<scalar<double> >, const scalar<double> > {

        void operator() (recursator<scalar<double> >& first, const scalar<double>& second)
        {
            std::cout << "Using my fancy new Assign function with only scalars" << std::endl;
            first(0, 0) = second(0, 0);
        }
    };

    template<typename Matrix1, typename Matrix2>
    struct Multiply {
        typedef row_iterator<Matrix1> iter_type1;
        typedef row_iterator<Matrix2> iter_type2;

        void operator() (Matrix1& first, Matrix2& second)
        {
            std::cout << "Using my fancy new Multiply function" << std::endl;
            for (unsigned y = 0; y < first.height(); ++y) {
                iter_type1 it1 = first.begin_row(y);
                iter_type2 it2 = second.begin_row(y);
                it1.set(it2);
                while (it2 != first.end_row()) {
                    *it1 *= *it2;
                    it1.set(++it2);
                }
            }
        }
    };

    template<typename Matrix1>
    struct Multiply<Matrix1, const scalar<typename Matrix1::value_type> > {
        typedef const scalar<typename Matrix1::value_type> scalar_type;
        typedef row_iterator<Matrix1> iter_type;

        void operator() (Matrix1& first, scalar_type& second)
        {
            std::cout << "Using my fancy new Multiply with a scalar" << std::endl;
            for (unsigned y = 0; y < first.height(); ++y) {
                iter_type it = first.begin_row(y);
                while (it != first.end_row()) {
                    *it *= second(0, 0);
                    ++it;
                }
            }
        }
    };

    template <>
    struct Multiply<scalar<double>, const scalar<double> > {

        void operator() (scalar<double>& first, const scalar<double>& second)
        {
            std::cout << "Using my fancy new Multiply function with only scalars" << std::endl;
            first(0, 0) *= second(0, 0);
        }
    };

    template<typename Matrix1, typename Matrix2>
    struct Divide {
        typedef row_iterator<Matrix1> iter_type1;
        typedef row_iterator<Matrix2> iter_type2;

        void operator() (Matrix1& first, Matrix2& second)
        {
            std::cout << "Using my fancy new Divide function" << std::endl;
            for (unsigned y = 0; y < first.height(); ++y) {
                iter_type1 it1 = first.begin_row(y);
                iter_type2 it2 = second.begin_row(y);
                it1.set(it2);
                while (it2 != first.end_row()) {
                    *it1 /= *it2;
                    it1.set(++it2);
                }
            }
        }
    };

    template<typename Matrix1>
    struct Divide<Matrix1, const scalar<typename Matrix1::value_type> > {
        typedef const scalar<typename Matrix1::value_type> scalar_type;
        typedef row_iterator<Matrix1> iter_type;

        void operator() (Matrix1& first, scalar_type& second)
        {
            std::cout << "Using my fancy new Divide function with a scalar" << std::endl;
            for (unsigned y = 0; y < first.height(); ++y) {
                iter_type it = first.begin_row(y);
                while (it != first.end_row()) {
                    *it /= second(0, 0);
                    ++it;
                }
            }
        }
    };

    template <>
    struct Divide<scalar<double>, const scalar<double> > {

        void operator() (scalar<double>& first, const scalar<double>& second)
        {
            std::cout << "Using my fancy new Divide function with only scalars" << std::endl;
            first(0, 0) /= second(0, 0);
        }
    };

    template<typename Operator, typename Matrix1, typename Matrix2>
    struct ApplyAndStore {
        typedef row_iterator<Matrix1> iter_type1;
        typedef row_iterator<Matrix2> iter_type2;

        void operator() (Operator& op, Matrix1& first, Matrix2& second)
        {
            std::cout << "Using my fancy new ApplyAndStore function" << std::endl;
            for (unsigned y = 0; y < first.height(); ++y) {
                iter_type1 it1 = first.begin_row(y);
                iter_type2 it2 = second.begin_row(y);
                it1.set(it2);
                while (it2 != first.end_row()) {
                    *it1 = op(*it2);
                    it1.set(++it2);
                }
            }
        }
    };

    template<typename Operator, typename Matrix1>
    struct ApplyAndStore<Operator, Matrix1, const sparse_matrix<typename Matrix1::value_type> > {
        typedef const sparse_matrix<typename Matrix1::value_type> sparse_type;
        typedef row_iterator<Matrix1> iter_type1;
        typedef row_iterator<sparse_type> iter_type2;

        void operator() (Operator& op, Matrix1& first, sparse_type& second)
        {
            std::cout << "Using my fancy new ApplyAndStore function with sparse matrices" << std::endl;
            for (unsigned y = 0; y < first.height(); ++y) {
                iter_type1 it1 = first.begin_row(y);
                iter_type2 it2 = second.begin_row(y);
                it1.set(it2);
                while (it2 != first.end_row()) {
                    *it1 = op(*it2);
                    it1.set(++it2);
                }
            }
        }
    };

    template<typename Operator, typename Matrix1>
    struct ApplyAndStore<Operator, Matrix1, const scalar<typename Matrix1::value_type> > {
        typedef const scalar<typename Matrix1::value_type> scalar_type;
        typedef row_iterator<Matrix1> iter_type;

        void operator() (Operator& op, Matrix1& first, scalar_type& second)
        {
            std::cout << "Using my fancy new ApplyAndStore function with a scalar" << std::endl;
            for (unsigned y = 0; y < first.height(); ++y) {
                iter_type it = first.begin_row(y);
                while (it != first.end_row()) {
                    *it = op(second(0, 0));
                    ++it;
                }
            }
        }
    };

    template<typename Operator>
    struct ApplyAndStore<Operator, scalar<double>, const scalar<double> > {

        void operator() (Operator& op, scalar<double>& first, const scalar<double>& second)
        {
            std::cout << "Using my fancy new ApplyAndStore function with only scalars" << std::endl;
            first(0, 0) = op(second(0, 0));
        }
    };
	
    // change to use the new std::function instead
    template <template <typename, typename> class Operator, typename Matrix1, typename Matrix2>
    void cast_and_apply(Matrix1& first, Matrix2& second)
    {
        typedef typename Matrix2::dense_type dense_type;
        typedef typename Matrix2::sparse_type sparse_type;
        typedef	typename Matrix2::scalar_type scalar_type;

        if (second.is_true_scalar()) { // kanske bara ta in värdet ist
            const scalar_type& sc = dynamic_cast<const scalar_type&> (second);
            Operator<Matrix1, const scalar_type> op;
            op(first, sc);
        }
        else if (second.is_dense()) {
            const dense_type& d = dynamic_cast<const dense_type&> (second);
            Operator<Matrix1, const dense_type> op;
            op(first, d);
        }
        else if (second.is_sparse()) {
            const sparse_type& s = dynamic_cast<const sparse_type&> (second);
            Operator<Matrix1, const sparse_type> op;
            op(first, s);
        }
        else {
            std::cout << "Did not match a type" << std::endl;
            Operator<Matrix1, const Matrix2> op;
            op(first, second);
        }
    }

    template <template <typename, typename, typename> class Operator, class Function, typename Matrix1, typename Matrix2>
    void cast_and_apply(Function& f, Matrix1& first, Matrix2& second)
    {
        typedef typename Matrix2::value_type value_type;
        typedef typename Matrix2::dense_type dense_type;
        typedef typename Matrix2::sparse_type sparse_type;
        typedef	typename Matrix2::scalar_type scalar_type;

        if (second.is_true_scalar()) { // kanske bara ta in värdet ist
            const scalar_type& sc = dynamic_cast<const scalar_type&> (second);
            Operator<Function, Matrix1, const scalar_type> op;
            op(f, first, sc);
        }
        else if (second.is_dense()) {
            const dense_type& d = dynamic_cast<const dense_type&> (second);
            Operator<Function, Matrix1, const dense_type> op;
            op(f, first, d);
        }
        else if (second.is_sparse()) {
            const sparse_type& s = dynamic_cast<const sparse_type&> (second);
            Operator<Function, Matrix1, const sparse_type> op;
            op(f, first, s);
        }
        else {
            Operator<Function, Matrix1, const Matrix2> op;
            op(f, first, second);
        }
    }
	
	template <typename Matrix1, typename Matrix2>
	void add_rows(Matrix1& first, const Matrix2& second)
    {
        cast_and_apply<Add>(first, second);
	}	
	
	template <typename Matrix1, typename Matrix2>
	void subtract_rows(Matrix1& first, const Matrix2& second)
	{
        cast_and_apply<Subtract>(first, second);
	}	
	
	template <typename Matrix1, typename Matrix2>
	void multiply_rows(Matrix1& first, const Matrix2& second)
	{
        cast_and_apply<Multiply>(first, second);
	}	
	
	template <typename Matrix1, typename Matrix2>
	void divide_rows(Matrix1& first, const Matrix2& second)
	{
        cast_and_apply<Divide>(first, second);
	}
	
	template <typename Matrix1, typename Matrix2>
	void assign_rows(Matrix1& first, const Matrix2& second)
	{
        cast_and_apply<Assign>(first, second);
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
	
    template <class Function, typename Matrix>
	Matrix* apply_function(Matrix& value)
    {
        Matrix* rtn = value.instantiate(value.height(), value.width());
        Function f;
        cast_and_apply<ApplyAndStore>(f, *rtn, value);
        return rtn;
	}
	
} // namespace logic
#endif
