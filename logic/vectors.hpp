#ifndef VECTORS_HPP
#define VECTORS_HPP

#include "operations.hpp"
#include "../util/convenience.hpp"

#include <cmath>
#include <cassert>

namespace logic {

	template<typename Matrix>
	class row_proxy {
	public:
		
		typedef Matrix matrix_type;
		typedef typename matrix_type::value_type value_type;
		typedef row_proxy self;

	private:
		
		matrix_type& value;
		unsigned row_nr;
		
	public:
		
		row_proxy(matrix_type& value, unsigned row_nr) : value(value), row_nr(row_nr)
		{
			assert(row_nr < value.height());
		}
		
		void operator= (const self& other)
		{
			value = other.value;
			row_nr = other.row_nr;
		}
		
		value_type operator[] (unsigned i) const
		{
			return value(row_nr, i);
		}
		
		value_type& operator[] (unsigned i)
		{
			return value(row_nr, i);
		}
		
		unsigned size() const { return value.width(); }
		
	}; // class row_proxy
	
	template<typename Matrix>
	class col_proxy {
	public:
		
		typedef Matrix matrix_type;
		typedef typename matrix_type::value_type value_type;
		typedef col_proxy self;

	private:
		
		matrix_type& value;
		unsigned col_nr;
		
	public:
		
		col_proxy(matrix_type& value, unsigned col_nr) : value(value), col_nr(col_nr)
		{
			assert(col_nr < value.width());
		}
		
		void operator= (const self& other)
		{
			value = other.value;
			col_nr = other.col_nr;
		}
		
		value_type operator[] (unsigned i) const
		{
			return value(i, col_nr);
		}
		
		value_type& operator[] (unsigned i)
		{
			return value(i, col_nr);
		}
		
		unsigned size() const { return value.height(); }
		
	}; // class col_proxy
	
	template<typename Operation, typename VectorType1, typename VectorType2>
	class operations_proxy {
		
		typedef VectorType1 vector_type1;
		typedef VectorType2 vector_type2;
		typedef operations_proxy self;
		
		vector_type1 first;
		vector_type1 second;
		
	public:
		
		operations_proxy(vector_type1& nfirst, vector_type2& nsecond) : first(nfirst), second(nsecond)
		{
			assert(first.size() == second.size());
		}
		
		template<typename VectorType>
		void store_in(VectorType& target) 
		{
			assert(first.size() == target.size());
			for (unsigned i = 0; i < target.size(); ++i) {
				target[i] = Operation()(first[i], second[i]);
			}
		}
		
	}; // class operations_proxy
	
	template<typename Vector>
	class vector_proxy {
	public:
		
		typedef Vector vector_type;
		typedef typename vector_type::value_type value_type;
		typedef vector_proxy self;
		
		vector_proxy(const vector_type& nvector) : vector(nvector) {}
		
		value_type operator[] (unsigned i) const
		{
			return vector[i];
		}
		
		value_type& operator[] (unsigned i)
		{
			return vector[i];
		}
		
		unsigned size() const { return vector.size(); }
		
		template<typename VectorType>
		value_type operator* (VectorType& other) 
		{
			assert(size() == other.size());
			value_type sum = 0;
			for (unsigned i = 0; i < size(); ++i) {
				sum += vector[i] * other[i];
			}
			return sum;
		}
		
		self& operator*= (value_type scalar) 
		{
			for (unsigned i = 0; i < size(); ++i) {
				vector[i] *= scalar;
			}
			return *this;
		}
		
		self& operator+= (value_type scalar) 
		{
			for (unsigned i = 0; i < size(); ++i) {
				vector[i] += scalar;
			}
			return *this;
		}
		
		self& operator-= (value_type scalar) 
		{
			for (unsigned i = 0; i < size(); ++i) {
				vector[i] -= scalar;
			}
			return *this;
		}
		
		self& operator^= (value_type scalar)
		{
			for (unsigned i = 0; i < size(); ++i) {
				vector[i] = pow(vector[i], scalar);
			}
			return *this;
		}
		
		template<typename VectorType>
		operations_proxy<plus<value_type>, self, VectorType> operator+ (VectorType other)
		{
			typedef operations_proxy<plus<value_type>, self, VectorType> my_oper;
			my_oper rtn(*this, other);
			return rtn;
		}
		
		template<typename VectorType>
		self& operator+= (const VectorType& other)
		{
			for (unsigned i = 0; i < size(); ++i) {
				vector[i] += other[i];
			}
			return *this;
		}
		
		template<typename VectorType>
		operations_proxy<minus<value_type>, self, VectorType> operator- (VectorType other)
		{
			typedef operations_proxy<minus<value_type>, self, VectorType> my_oper;
			my_oper rtn(*this, other);
			return rtn;
		}
		
		template<typename VectorType>
		self& operator-= (const VectorType& other)
		{
			for (unsigned i = 0; i < size(); ++i) {
				vector[i] -= other[i];
			}
			return *this;
		}
		
		void apply_function(value_type (*f)(value_type))
		{
			for (unsigned i = 0; i < size(); ++i) {
				vector[i] = (*f)(vector[i]);
			}
		}
		
		template<typename Operation, typename VectorType1, typename VectorType2>
		self& operator= (operations_proxy<Operation, VectorType1, VectorType2> op)
		{
			op.store_in(*this);
			return *this;
		}
		
		template<typename OtherVector>
		self& operator= (const vector_proxy<OtherVector>& other)
		{
			assert(size() == other.size());
			for (unsigned i = 0; i < size(); ++i) {
				vector[i] = other[i];
			}
			return *this;
		}
		
		template<typename OutStream>
		void output_as_string(OutStream& s)
		{
			for (unsigned i = 0; i < size(); ++i) {
				s << (char)vector[i];
			}
		}
		
	private:
		
		vector_type vector;
		
	}; // class vector_proxy
	
	template<typename OutStream, typename Vector>
	std::ostream& operator<< (OutStream& s, const vector_proxy<Vector>& v)
	{
		unsigned size = v.size();
		for (unsigned i = 0; i < size; ++i) {
			s << v[i] << ' ';
		}
		return s;
	}

} // namespace logic
#endif