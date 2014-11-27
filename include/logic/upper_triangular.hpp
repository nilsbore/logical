#ifndef UPPER_TRIANGULAR_HPP
#define UPPER_TRIANGULAR_HPP

namespace logic {

	template <typename Value>
	class upper_triangular : public virtual_matrix<Value> {
	private:
		
		typedef upper_triangular self;
		typedef virtual_matrix<Value> super;
		
	public:
		
		typedef Value value_type;
		
		unsigned height() const { return rows; }
		unsigned width() const { return cols; }
		
		value_type& operator() (unsigned y, unsigned x)
		{
			if ((int)x - (int)y < 0) {
				return zero;
			}
			return vals[y][x - y];
		}
		
		const value_type& operator() (unsigned y, unsigned x) const
		{
			if ((int)x - (int)y < 0) {
				return zero;
			}
			return vals[y][x - y];
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
		
		super* product(const super& other) const {
			return matrix_product(*this, other);
		}
		
		void transp() {}
		
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
		
		~dense_matrix()
		{
			for (unsigned i = 0; i < rows; ++i) {
				delete[] vals[i];
			}
			delete[] vals;			
		}
		
	private:
		
		static value_type zero;
		unsigned rows, cols;
		value_type** vals;
	};
	
	template<typename Value>
	Value upper_triangular<Value>::zero = Value(0);
	
} // namespace logic