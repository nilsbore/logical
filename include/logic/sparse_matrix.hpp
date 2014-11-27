#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include "virtual_matrix.hpp"
#include "matrix_operations.hpp"
#include "recursator.hpp"

namespace logic {
	
	template <typename Value>
	class sparse_matrix : public virtual_matrix<Value> {
	private:
		
		typedef sparse_matrix self;
		
	public:
		
		typedef virtual_matrix<Value> super;
		typedef Value value_type;
		
		template <typename> friend class sparse_row_iterator;
		template <typename> friend class sparse_col_iterator;
		template <typename> friend class row_iterator;
		template <typename> friend class col_iterator;
		
	private:
		
		struct node {
			node *down, *right;
			unsigned y, x;
			value_type val;
		};
		
		void init_node(node& target, node* right, node* down, 
					   unsigned y, unsigned x, value_type val)
		{
			target.right = right;
			target.down = down;
			target.y = y;
			target.x = x;
			target.val = val;
		}
		
		void insert_in_row(node* n) {
			node *temp, *previous;
			temp = row_vals[n->y];
			if (temp == NULL) {
				row_vals[n->y] = n;
				n->right = NULL;
				return;
			}
			if (temp->x > n->x) {
				previous = row_vals[n->y] = n;
				previous->right = temp;
				return;
			}
			do {
				previous = temp;
				temp = temp->right;
			}
			while (temp != NULL && temp->x < n->x);
			previous->right = n;
			n->right = temp;
		}
		
		void insert_in_col(node* n) {
			node *temp, *previous;
			temp = col_vals[n->x];
			if (temp == NULL) {
				col_vals[n->x] = n;
				n->down = NULL;
				return;
			}
			if (temp->y > n->y) {
				previous = col_vals[n->x] = n;
				previous->down = temp;
				return;
			}
			do {
				previous = temp;
				temp = temp->down;
			}
			while (temp != NULL && temp->y < n->y);
			previous->down = n;
			n->down = temp;
		}
		
	public:
		
		unsigned height() const { return rows; }
		unsigned width() const { return cols; }
		
		value_type& operator() (unsigned y, unsigned x)
		{ 
			node *temp, *previous;
			
			if (rows <= cols) {
				temp = col_vals[x];
				if (temp == NULL) {
					temp = col_vals[x] = new node;
					init_node(*temp, NULL, NULL, y, x, value_type(0));
					insert_in_row(temp);
					return temp->val;
				}
				if (temp->y == y) {
					return temp->val;
				}
				if (temp->y > y) {
					previous = col_vals[x] = new node;
					init_node(*previous, NULL, temp, y, x, value_type(0));
					insert_in_row(previous);
					return previous->val;
				}
				do {
					previous = temp;
					temp = temp->down;
				}
				while (temp != NULL && temp->y < y);
				if (temp != NULL && temp->y == y) {
					return temp->val;
				}
				previous->down = new node;
				previous = previous->down;
				init_node(*previous, NULL, temp, y, x, value_type(0));
				insert_in_row(previous);
				return previous->val;
			}
			
			temp = row_vals[y];
			if (temp == NULL) {
				row_vals[y] = new node;
				temp = row_vals[y];
				init_node(*temp, NULL, NULL, y, x, value_type(0));
				insert_in_col(temp);
				return temp->val;
			}
			if (temp->x == x) {
				return temp->val;
			}
			if (temp->x > x) {
				previous = row_vals[y] = new node;
				init_node(*previous, temp, NULL, y, x, value_type(0));
				insert_in_col(previous);
				return previous->val;
			}
			do {
				previous = temp;
				temp = temp->right;
			}
			while (temp != NULL && temp->x < x);
			if (temp != NULL && temp->x == x) {
				return temp->val;
			}
			previous->right = new node;
			previous = previous->right;
			init_node(*previous, temp, NULL, y, x, value_type(0));
			insert_in_col(previous);
			return previous->val;
		}
		
		const value_type& operator() (unsigned y, unsigned x) const
		{
			node* temp;
			if (rows <= cols) {
				temp = col_vals[x];
				while (temp != NULL && temp->y < y) {
					temp = temp->down;
				}
				if (temp != NULL && temp->y == y) {
					return temp->val;
				}
				return zero;
			}
			temp = row_vals[y];
			while (temp != NULL && temp->x < x) {
				temp = temp->right;
			}
			if (temp != NULL && temp->x == x) {
				return temp->val;
			}
			return zero;
		}
		
		value_type& operator[] (unsigned i)
		{
			if (cols == 1) {
				return (*this)(i, 1);
			}
			return (*this)(1, i);
		}
		
		const value_type& operator[] (unsigned i) const
		{
			if (cols == 1) {
				return (*this)(i, 1);
			}
			return (*this)(1, i);
		}
		
		// Operators for matrices
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
		
		super* product(const super& other) const
		{
			return matrix_product(*this, other);
		}
		
		// Operators for non-constant recursators
		super& operator+= (const virtual_recursator<value_type, false>& other)
		{
			add_rows(*this, other);
			return *this;
		}
		
		super& operator-= (const virtual_recursator<value_type, false>& other)
		{
			subtract_rows(*this, other);
			return *this;
		}
		
		super& operator*= (const virtual_recursator<value_type, false>& other)
		{
			multiply_rows(*this, other);
			return *this;
		}
		
		super& operator/= (const virtual_recursator<value_type, false>& other)
		{
			divide_rows(*this, other);
			return *this;
		}
		
		super* product(const virtual_recursator<value_type, false>& other) const
		{
			return matrix_product(*this, other);
		}
		
		// Operators for constant recursators
		super& operator+= (const virtual_recursator<value_type, true>& other)
		{
			add_rows(*this, other);
			return *this;
		}
		
		super& operator-= (const virtual_recursator<value_type, true>& other)
		{
			subtract_rows(*this, other);
			return *this;
		}
		
		super& operator*= (const virtual_recursator<value_type, true>& other)
		{
			multiply_rows(*this, other);
			return *this;
		}
		
		super& operator/= (const virtual_recursator<value_type, true>& other)
		{
			divide_rows(*this, other);
			return *this;
		}
		
		super* product(const virtual_recursator<value_type, true>& other) const
		{
			return matrix_product(*this, other);
		}
		
		self& operator= (const self& other)
		{
			typedef assign<row_iterator<self>, row_iterator<const self> > oper_type;
			
			for_each_row<oper_type>(*this, other);
			return *this;
		}
		
		/*super& operator= (const super& other)
		{
			return *this;
		}*/
		
		void transp() {
			node *temp1;
			for (unsigned y = 0; y < rows; ++y) {
				temp1 = row_vals[y];
				while (temp1 != NULL) {
					init_node(*temp1, temp1->down, temp1->right,
							  temp1->x, temp1->y, temp1->val);
					temp1 = temp1->down;
				}
			}
			unsigned temp2 = cols;
			cols = rows;
			rows = temp2;
			node** temp3 = col_vals;
			col_vals = row_vals;
			row_vals = temp3;
		}
		
		typename super::recursator_type* new_recursator(unsigned firstrow, unsigned lastrow,
														unsigned firstcol, unsigned lastcol)
		{
			return new recursator<self>(*this, firstrow, lastrow, firstcol, lastcol);
		}
		
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
		
        super* instantiate(size_t rows, size_t cols) const
        {
            return new sparse_matrix(rows, cols);
        }

        super* clone() const
        {
            return new sparse_matrix(*this);
        }

		sparse_matrix(const self& other) : super(super::SPARSE),
										   rows(other.rows), cols(other.cols)
		{
			typedef assign<row_iterator<self>, row_iterator<const self> > oper_type;
			
			row_vals = new node*[rows];
			col_vals = new node*[cols];
			for (unsigned y = 0; y < rows; ++y) {
				row_vals[y] = NULL;
			}
			for (unsigned x = 0; x < cols; ++x) {
				col_vals[x] = NULL;
			}
			for_each_row<oper_type>(*this, other);
		}
		
		sparse_matrix(const value_type& val, unsigned size, int) : super(super::SPARSE),
														rows(size), cols(size)
		{
			row_vals = new node*[size];
			col_vals = new node*[size];
			for (unsigned i = 0; i < size; ++i) {
				row_vals[i] = col_vals[i] = new node;
				init_node((*row_vals[i]), NULL, NULL, i, i, val);
			}
		}
		
		sparse_matrix(unsigned rows, unsigned cols) : super(super::SPARSE),
													  rows(rows), cols(cols)
		{
			row_vals = new node*[rows];
			col_vals = new node*[cols];
			for (unsigned y = 0; y < rows; ++y) {
				row_vals[y] = NULL;
			}
			for (unsigned x = 0; x < cols; ++x) {
				col_vals[x] = NULL;
			}
		}
		
		~sparse_matrix()
		{
			node *temp, *previous;
			for (unsigned y = 0; y < rows; ++y) {
				temp = row_vals[y];
				while (temp != NULL) {
					previous = temp;
					temp = temp->right;
					delete previous;
				}
			}
			delete[] row_vals;
			delete[] col_vals;
		}
		
	private:
		
		unsigned rows, cols;
		node** row_vals;
		node** col_vals;
		static value_type zero;
	};	
	
	template<typename Value>
	Value sparse_matrix<Value>::zero = Value(0);
	
	template<typename Sparse>
	class sparse_row_iterator {
	public:
		
		typedef Sparse matrix_type;
		typedef typename Sparse::value_type value_type;
		
	protected:
		
		typedef sparse_row_iterator self;
		typedef typename util::if_<util::is_const<Sparse>,
		const Sparse&, Sparse&>::type ref_type;
		typedef typename Sparse::node node;
		
	public:
		
		unsigned pos() const {
			if (is_zero) {
				return x;
			}
			if (current != NULL) {
				return current->x;
			}
			return val.width();
		}
		
		void explicit_set(unsigned i)
		{
			x = i;
			node* previous;
			
			if (x >= val.width()) {
				current = NULL;
				x = val.width();
				is_zero = false;
				return;
			}
			if (current == NULL || current->x > x) {
				current = val.row_vals[row];
				if (current == NULL || current->x > x) {
					current = NULL;
					is_zero = true;
					return;
				}
			}
			if (current->x == x) {
				is_zero = false;
				return;
			}
			do {
				previous = current;
				current = current->right;
			}
			while (current != NULL && current->x < x);
			if (current != NULL && current->x == x) {
				is_zero = false;
				return;
			}
			current = previous;
			is_zero = true;
		}
		
		template <typename Iterator>
		void set(const Iterator& other) { explicit_set(other.pos()); }
		
		const value_type& operator* () const {
			if (is_zero) {
				return val.zero;
			}
			return current->val;
		}
		
		void next ()
		{
			if (current != NULL) {
				current = current->right;
			}
			else if (is_zero) {
				current = val.row_vals[row];
			}
			is_zero = false;
		}
		
		bool operator!= (unsigned end) const { 
			return (is_zero && x < end) || (current != NULL && current->x < end);
		}
		
		sparse_row_iterator(ref_type val, unsigned row) : is_zero(false), row(row), val(val),
												current(val.row_vals[row]) {}
		sparse_row_iterator(const self& other) : is_zero(other.is_zero), row(other.row), val(other.val),
												current(other.current) {}
	protected:
		
		bool is_zero;
		unsigned row, x;
		node* current;
		ref_type val;
	};
	
	template <typename Value>
	class row_iterator<sparse_matrix<Value> > : public sparse_row_iterator<sparse_matrix<Value> > {
	public:
		
		typedef Value value_type;
		
	private:
		
		typedef row_iterator self;
		typedef sparse_row_iterator<sparse_matrix<Value> > super;
		typedef typename super::ref_type ref_type;
		typedef value_type& access_type;
		typedef typename super::node node;
		
	public:
		
		self& operator++ ()
		{
			super::next();
			return *this;
		}
		
		access_type operator* () {
			if (super::is_zero) {
				if (super::current == NULL) {
					super::current = new node;
					super::val.init_node(*super::current, super::val.row_vals[super::row],
										 NULL, super::row, super::x, value_type(0));
					super::val.insert_in_col(super::current);
					super::val.row_vals[super::row] = super::current;
				}
				else {
					node* temp = new node;
					super::val.init_node(*temp, super::current->right, NULL,
										 super::row, super::x, value_type(0));
					super::current->right = temp;
					super::current = temp;
					super::val.insert_in_col(super::current);
				}
				super::is_zero = false;
			}
			return super::current->val;
		}
		
		row_iterator(ref_type val, unsigned row) : super(val, row) {}
		row_iterator(const self& other) : super(other) {}
		
	};
	
	template <typename Value>
	class row_iterator<const sparse_matrix<Value> > : public sparse_row_iterator<const sparse_matrix<Value> >  {
	public:
		
		typedef Value value_type;
		
	private:
		
		typedef row_iterator self;
		typedef sparse_row_iterator<const sparse_matrix<Value> > super;
		typedef typename super::ref_type ref_type;
		
	public:
		
		self& operator++ ()
		{
			super::next();
			return *this;
		}
		
		row_iterator(ref_type val, unsigned row) : super(val, row) {}
		row_iterator(const self& other) : super(other) {}
		
	};
	
	template<typename Sparse>
	class sparse_col_iterator {
	public:
		
		typedef typename Sparse::value_type value_type;
		
	protected:
		
		typedef sparse_col_iterator self;
		typedef typename util::if_<util::is_const<Sparse>,
		const Sparse&, Sparse&>::type ref_type;
		typedef typename Sparse::node node;
		
	public:
		
		unsigned pos() const {
			if (is_zero) {
				return y;
			}
			if (current != NULL) {
				return current->y;
			}
			return val.height();
		}
		
		void explicit_set(unsigned i)
		{
			y = i;
			node* previous;
			
			if (y >= val.height()) {
				current = NULL;
				y = val.height();
				is_zero = false;
				return;
			}
			if (current == NULL || current->y > y) {
				current = val.col_vals[col];
				if (current == NULL || current->y > y) {
					current = NULL;
					is_zero = true;
					return;
				}
			}
			if (current->y == y) {
				is_zero = false;
				return;
			}
			do {
				previous = current;
				current = current->down;
			}
			while (current != NULL && current->y < y);
			if (current != NULL && current->y == y) {
				is_zero = false;
				return;
			}
			current = previous;
			is_zero = true;
		}
		
		template <typename Iterator>
		void set(const Iterator& other) { explicit_set(other.pos()); }
		
		void next ()
		{
			if (current != NULL) {
				current = current->down;
			}
			else if (is_zero) {
				current = val.col_vals[col];
			}
			is_zero = false;
		}
		
		bool operator!= (unsigned end) const {
			return (is_zero && y < end) || (current != NULL && current->y < val.height());
		}
		
		const value_type& operator* () const
		{
			if (is_zero) {
				return val.zero;
			}
			return current->val;
		}
		
		sparse_col_iterator(ref_type val, unsigned col) : is_zero(false), col(col), 
										val(val), current(val.col_vals[col]) {}
		sparse_col_iterator(const self& other) : is_zero(other.is_zero), col(other.col),
										val(other.val), current(other.current) {}
	protected:
		
		bool is_zero;
		unsigned col, y;
		node* current;
		ref_type val;
	};
	
	template <typename Value>
	class col_iterator<sparse_matrix<Value> > : public sparse_col_iterator<sparse_matrix<Value> > {
	public:
		
		typedef Value value_type;
		
	private:
		
		typedef col_iterator self;
		typedef sparse_col_iterator<sparse_matrix<Value> > super;
		typedef typename super::ref_type ref_type;
		typedef value_type& access_type;
		typedef typename super::node node;
		
	public:
		
		self& operator++ ()
		{
			super::next();
			return *this;
		}
		
		access_type operator* () {
			if (super::is_zero) {
				if (super::current == NULL) {
					super::current = new node;
					super::val.init_node(*super::current, NULL, super::val.col_vals[super::col],
										 super::y, super::col, value_type(0));
					super::val.insert_in_row(super::current);
					super::val.row_vals[super::row] = super::current;
				}
				else {
					node* temp = new node;
					super::val.init_node(*temp, NULL, super::current->down,
										 super::y, super::col, value_type(0));
					super::current->down = temp;
					super::current = temp;
					super::val.insert_in_row(super::current);
				}
				super::is_zero = false;
			}
			return super::current->val;
		}
		
		col_iterator(ref_type val, unsigned col) : super(val, col) {}
		col_iterator(const self& other) : super(other) {}
		
	};
	
	template <typename Value>
	class col_iterator<const sparse_matrix<Value> > : public sparse_col_iterator<const sparse_matrix<Value> >  {
	public:
		
		typedef Value value_type;
		
	private:
		
		typedef col_iterator self;
		typedef sparse_col_iterator<const sparse_matrix<Value> > super;
		typedef typename super::ref_type ref_type;
		
	public:
		
		self& operator++ ()
		{
			super::next();
			return *this;
		}
		
		col_iterator(ref_type val, unsigned col) : super(val, col) {}
		col_iterator(const self& other) : super(other) {}
		
	};

} // namespace logic
#endif
