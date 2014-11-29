#ifndef BUILD_PROXY_HPP
#define BUILD_PROXY_HPP

#include "../util/string.hpp"
#include "../util/info_auto_ptr.hpp"

template <typename Value>
struct build_node {
	
	typedef build_node self;
	typedef Value value_type;
	
	self* next_node;
	value_type* val;
	
    build_node() : next_node(NULL), val(NULL) {} // val(new value_type()) {}
	~build_node()
	{
        delete next_node; delete val;
	}
};

template <typename Matrix>
class build_proxy {
public:
	
    typedef Matrix matrix_type;
    typedef build_node<matrix_type> matrix_node;
	typedef build_node<matrix_node> row_node;
    typedef util::info_auto_ptr<matrix_type, util::string> return_type;
	
    build_proxy(return_type& top)
	{
		is_string = top->is_string();
		rows = top->height();
		current_cols = cols = top->width();
        current_row = first = new row_node;
        first->val = new matrix_node;
        current_matrix = first->val;
        current_matrix->val = top.release();
		current_matrix->next_node = NULL;
		first->next_node = NULL;
        first_row = true;
	}
	
	~build_proxy()
	{
		delete first;
	}
	
	return_type build()
	{
		if (current_cols != cols) {
			return "Last row doesn't agree with matrix dimensions.";
		}
        matrix_type* rtn = first->val->val->instantiate(rows, cols);
		current_row = first;
		current_matrix = first->val;
		unsigned current_i, current_j;
		current_i = 0;
        // change to use recursators instead
		for (unsigned i = 0; i < rows; ++i, ++current_i) {
			if (current_i >= current_matrix->val->height()) {
				current_row = current_row->next_node;
				current_matrix = current_row->val;
				current_i = 0;
			}
			current_j = 0;
			current_matrix = current_row->val;
			for (unsigned j = 0; j < cols; ++j, ++current_j) {
				if (current_j >= current_matrix->val->width()) {
					current_matrix = current_matrix->next_node;
					current_j = 0;
				}
				(*rtn)(i, j) = (*(current_matrix->val))(current_i, current_j);
			}
		}
		// rtn->set_string(is_string); TODO: initialize as string(or sparse)
		return return_type(rtn);
	}
	
	/*void print()
	{
		current_row = first;
		current_matrix = &(first->val);
		unsigned current_i, current_j;
		current_i = 0;
		for (unsigned i = 0; i < rows; ++i, ++current_i) {
			if (current_i >= current_matrix->val.height()) {
				current_row = current_row->next_node;
				current_matrix = &(current_row->val);
				current_i = 0;
			}
			current_j = 0;
			current_matrix = &(current_row->val);
			for (unsigned j = 0; j < cols; ++j, ++current_j) {
				if (current_j >= current_matrix->val.width()) {
					current_matrix = current_matrix->next_node;
					current_j = 0;
				}
				std::cout << current_matrix->val(current_i, current_j) << ' ';
			}
			std::cout << std::endl;
		}
	}
	
	void print_parts()
	{
		current_row = first;
		while(current_row != NULL) {
			current_matrix = &(current_row->val);
			while (current_matrix != NULL) {
				std::cout << current_matrix->val;
				current_matrix = current_matrix->next_node;
			} 
			current_row = current_row->next_node;
		}
	}*/
	
    bool add_to_right(return_type& right)
	{
		if (right->height() != current_matrix->val->height()) return false;
		if (!first_row && current_cols + right->width() > cols) return false;
		is_string = is_string && right->is_string();
		current_cols += right->width();
		if (first_row) {
			cols = current_cols;
		}
		current_matrix->next_node = new matrix_node;
		current_matrix = current_matrix->next_node;
		current_matrix->val = right.release();
		current_matrix->next_node = NULL;
		return current_matrix->val != NULL;
	}
	
    bool add_to_bottom(return_type& bottom)
	{
		if (current_cols != cols || bottom->width() > cols) return false;
		is_string = is_string && bottom->is_string();
		current_cols = bottom->width();
		rows += bottom->height();
		first_row = false;
		current_row->next_node = new row_node;
		current_row = current_row->next_node;
		current_row->next_node = NULL;
        current_row->val = new matrix_node;
		current_matrix = current_row->val;
		current_matrix->val = bottom.release();
		current_matrix->next_node = NULL;
		return current_matrix->val != NULL;
	}
	
private:

	bool first_row, is_string;
	unsigned rows, cols, current_cols;
	row_node *first, *current_row;
	matrix_node* current_matrix;
	
};
#endif
