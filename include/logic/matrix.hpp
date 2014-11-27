#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vectors.hpp"
#include "../util/convenience.hpp"
#include "../util/string.hpp"

#include <cassert>

namespace logic {
	
	template <typename Value, unsigned Cache = 256>
	class matrix {
	public:
		
		typedef Value value_type;
		typedef typename util::get_base_type<value_type>::type base_type;
		typedef matrix self;
		typedef row_proxy<self> row_proxy;
		typedef col_proxy<self> col_proxy;
		typedef vector_proxy<row_proxy> row;
		typedef vector_proxy<col_proxy> col;
		
		const static unsigned cache_size = Cache;
		
		matrix(unsigned rows, unsigned cols) : is_matrix(true), rows(rows), cols(cols) 
		{
			vals = new value_type*[rows];
			for (unsigned y = 0; y < rows; ++y) {
				vals[y] = new value_type[cols];
			}
		}
		
		matrix(const self& other) : is_matrix(other.is_matrix), rows(other.rows), 
									cols(other.cols), vals(other.vals)
		{	
			//assert(other.vals != NULL);
			other.rows = 0;
			other.cols = 0;
			other.vals = NULL;
		}
		
		matrix(const util::string& value) : is_matrix(false), rows(1), cols(value.length())
		{
			assert(cols != 0);
			vals = new value_type*[1];
			*vals = new value_type[cols];
			for (unsigned i = 0; i < cols; ++i) {
				(*this)(0, i) = value[i];
			}
		}
		
		matrix() : is_matrix(true), rows(0), cols(0), vals(NULL) {}
		
		self& operator= (const self& other)
		{	
			//assert(other.vals != NULL);
			
			is_matrix = other.is_matrix;
			
			// delete old values
			if (other.vals != vals) {
				for (unsigned i = 0; i < rows; ++i) {
					delete[] vals[i];
				}
				delete[] vals;
			}
			
			// take values from other matrix
			rows = other.rows;
			cols = other.cols;
			vals = other.vals;
			
			// delete references in other matrix
			other.rows = 0;
			other.cols = 0;
			other.vals = NULL;
		}
				
		self clone() const
		{
			matrix rtn;
			rtn.is_matrix = is_matrix;
			rtn.rows = rows;
			rtn.cols = cols;
			rtn.vals = new value_type*[rows];
			for (unsigned y = 0; y < rows; ++y) {
				rtn.vals[y] = new value_type[cols];
				for (unsigned x = 0; x < cols; ++x) {
					rtn(y, x) = (*this)(y, x);
				}
			}
			return rtn;
		}
		
		bool is_string() const
		{
			return !is_matrix;
		}
		
		void set_string(bool nis_string)
		{
			is_matrix = !nis_string;
		}
		
		bool is_scalar() const
		{
			return rows == 1 && cols == 1;
		}
		
		bool is_vector() const
		{
			return rows == 1 || cols == 1;
		}
		
		bool is_empty() const
		{
			return vals == NULL;
		}
		
		unsigned length() const
		{
			assert(is_vector());
			return rows > cols ? rows : cols;
		}
		
		static matrix real_scalar(base_type val)
		{
			self rtn(1, 1);
			rtn(0, 0) = val;
			return rtn;
		}
		
		void clear()
		{
			for (unsigned i = 0; i < rows; ++i) {
				delete[] vals[i];
			}
			delete[] vals;
			rows = 0;
			cols = 0;
			is_matrix = true;
		}		
		
		~matrix() 
		{
			for (unsigned i = 0; i < rows; ++i) {
				delete[] vals[i];
			}
			delete[] vals;			
		}
		
		value_type& operator() (unsigned y, unsigned x) 
		{
			assert(y < rows && x < cols);
			return vals[y][x];
		}
		
		value_type operator() (unsigned y, unsigned x) const
		{
			assert(y < rows && x < cols);
			return vals[y][x];
		}
		
		value_type& operator[] (unsigned i) 
		{
			assert(is_vector());
			if (rows == 1) {
				assert(i < cols);
				return vals[0][i];
			}
			assert(i < rows);
			return vals[i][0];
		}
		
		value_type operator[] (unsigned i) const
		{
			assert(is_vector());
			if (rows == 1) {
				assert(i < cols);
				return vals[0][i];
			}
			assert(i < rows);
			return vals[i][0];
		}
		
		unsigned height() const { return rows; }
		unsigned width() const { return cols; }
		
		row get_row(unsigned y) { return row(row_proxy(*this, y)); }
		col get_col(unsigned x) { return col(col_proxy(*this, x)); }
		
		/*row get_row(unsigned y) const
		{ 
			return row(row_proxy(*this, y));
		}*/
		
		self operator* (self& other)
		{
			if (is_scalar()) {
				self temp = other.clone();
				temp *= (*this)(0, 0);
				return temp;
			}
			else if (other.is_scalar()) {
				self temp = clone();
				temp *= other(0, 0);
				return temp;
			}
			if (cols != other.height()) {
				return self();
			}
			self rtn(rows, other.width());
			for (unsigned y = 0; y < rows; ++y) {
				for (unsigned x = 0; x < other.width(); ++x) {
					row r = get_row(y);
					col c = other.get_col(x);
					rtn(y, x) = r * c;
				}
			}
			return rtn;
		}
		
		bool std_bin_operator_dim_agree(self& other)
		{
			if (is_scalar() || other.is_scalar()) {
				return true;
			}
			return rows == other.height() && cols == other.width();
		}
		
		self operator+ (self& other)
		{
			assert(rows == other.height() && cols == other.width());
			self rtn(rows, cols);
			rtn.is_matrix = is_matrix || other.is_matrix;
			for (unsigned y = 0; y < rows; ++y) {
				row r = rtn.get_row(y);
				r = get_row(y) + other.get_row(y);
			}
			return rtn;
		}
		
		self& operator+= (self& other)
		{
			if (other.is_scalar()) {
				(*this) += other(0, 0);
				return *this;
			}
			else if (is_scalar()) {
				value_type temp = (*this)(0, 0);
				(*this) = other.clone();
				(*this) += temp;
				return *this;
			}
			
			/*if (rows != other.height() || cols != other.width()) {
				return *this;
			}*/
			
			for (unsigned y = 0; y < rows; ++y) {
				row r = get_row(y);
				r += other.get_row(y);
			}
			return *this;
		}
		
		self operator- (self& other)
		{
			assert(rows == other.height() && cols == other.width());
			self rtn(rows, cols);
			rtn.is_matrix = is_matrix || other.is_matrix;
			for (unsigned y = 0; y < rows; ++y) {
				row r = rtn.get_row(y);
				r = get_row(y) - other.get_row(y);
			}
			return rtn;
		}
		
		self& operator-= (self& other)
		{
			if (other.is_scalar()) {
				(*this) -= other(0, 0);
				return *this;
			}
			else if (is_scalar()) {
				value_type temp = (*this)(0, 0);
				(*this) = other.clone();
				(*this) *= -1;
				(*this) += temp;
				return *this;
			}

			/*if (rows != other.height() || cols != other.width()) {
				return *this;
			}*/
			
			for (unsigned y = 0; y < rows; ++y) {
				row r = get_row(y);
				r -= other.get_row(y);
			}
			return *this;
		}
		
		self& operator^= (self& other)
		{
			(*this) ^= other(0, 0);
			return *this;
		}
		
		self transp()
		{
			self rtn(cols, rows);
			for (unsigned y = 0; y < rows; ++y) {
				col c = rtn.get_col(y);
				c = get_row(y);
			}
			return rtn;
		}
		
		self& operator*= (value_type scalar)
		{
			for (unsigned y = 0; y < rows; ++y) {
				get_row(y) *= scalar;
			}
			return *this;
		}
		
		self& operator+= (value_type scalar)
		{
			for (unsigned y = 0; y < rows; ++y) {
				get_row(y) += scalar;
			}
			return *this;
		}
		
		self& operator-= (value_type scalar)
		{
			for (unsigned y = 0; y < rows; ++y) {
				get_row(y) -= scalar;
			}
			return *this;
		}
		
		self& operator^= (value_type scalar)
		{
			for (unsigned y = 0; y < rows; ++y) {
				get_row(y) ^= scalar;
			}
			return *this;
		}
		
		void apply_function(value_type (*f)(value_type))
		{
			for (unsigned y = 0; y < rows; ++y) {
				get_row(y).apply_function(f);
			}
		}

	private:
		mutable unsigned rows, cols;
		mutable value_type** vals;
		mutable bool is_matrix;
	}; // class matrix
	
	template<typename OutStream, typename Value, unsigned Cache>
	OutStream& operator<< (OutStream& s, matrix<Value, Cache>& m)
	{
		typedef typename matrix<Value>::row row;
		for (unsigned i = 0; i < m.height(); ++i) {
			row r = m.get_row(i);
			if (m.is_string()) {
				r.output_as_string(s);
			}
			else {
				s << r;
			}
			s << '\n';
		}
		return s;
	}
	
} // namespace logic
#endif