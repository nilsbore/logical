#ifndef VAR_EVALUATOR_HPP
#define VAR_EVALUATOR_HPP

#include <cassert>
#include "base_evaluator.hpp"
#include "util/map.hpp"
#include "util/string.hpp"

using namespace logic;

template <typename Value, typename OutStream>
class var_evaluator : public base_evaluator<Value, OutStream> {
protected:
	
	typedef var_evaluator self;
	typedef Value value_type;
	typedef OutStream out_stream;
	typedef	base_evaluator<Value, OutStream> base;
    typedef typename base::matrix_type matrix_type;
    typedef typename base::dense_type dense_type;
    typedef typename base::sparse_type sparse_type;
    typedef typename base::scalar_type scalar_type;
    typedef util::map<util::string, matrix_type> var_map;

public:

    typedef typename base::return_type return_type;

protected:

	return_type name(const util::string& arg)
	{
		return_type temp = base::name(arg);
		if (temp.valid()) {
			return temp;
		}
        matrix_type* rtn = variables[arg];
        if (rtn->is_empty()) return "Variable not found.";
        return rtn->clone();
	}
	
	virtual void add_to_namespace(util::string& varname) {}
		
	return_type assignment()
	{
		base::input.next_blanks();
		if(!base::input.is_letter()) return "Assignment right hand side not variable.";
		util::string varname = base::input.next_name();
		if (*base::input == '[') {
			base::input++;
			return index_assignment(varname);
		}
		base::input.next_blanks();
		if (*base::input != '=') return "Can't read right hand side of assignment.";
		base::input++;
		return_type value = base::build_matrix(';', ';', true);
		if (!value.valid()) return value;
        matrix_type* varvalue = value->clone();
        util::string temp = varname;
		if (variables.insert(temp, varvalue)) {
			add_to_namespace(varname);
		}
		return value;
	}
	
	return_type index_assignment(const util::string& varname)
	{
        matrix_type* var = variables[varname];
		if (var == NULL) return "Right hand side variable not found.";
		unsigned previousend = base::index_end;
		if (var->is_vector()) {
			base::index_end = var->length() - 1;
		}
		else {
			base::index_end = var->height() - 1;
		}
		return_type first = base::build_matrix(',', ']', false);
		if (!first.valid()) return first;
		return_type value;
		if (*base::input == ']') {
			base::index_end = previousend;
			base::input++;
			base::input.next_blanks();
			assert(*base::input == '=');
			base::input++;
			value = base::build_matrix(';', ';', true);
			if (!value.valid()) return value;
            if(!assign_index(*var, *first, *value)) return "Assignment parameters don't match.";
			return var->clone();
		}
		if (*base::input != ',') return "Missing ',' in index argument.";
		base::input++;
		base::input.next_blanks();
		base::index_end = var->width() - 1;
		return_type second = base::build_matrix(']', ']', false);
		if (!second.valid()) return second;
		base::index_end = previousend;
		if (*base::input != ']') return "Index expression not terminated correctly.";
		base::input++;
		base::input.next_blanks();
		if (*base::input != '=') return "Can't read right hand side of assignment.";
		base::input++;
		value = base::build_matrix(';', ';', true);
		if (!value.valid()) return value;
        if (!assign_index(*var, *first, *second, *value)) return "Assignment parameters don't match.";
		return var->clone();
	}
	
public:
	
	return_type evaluate(const char* arg, int length)
	{
		base::input.new_expression(arg, length);
		return_type rtn;
		
		if (base::input.contains_def()) {
			rtn = assignment();
		}
		else {
			rtn = base::build_matrix(';', ';', true);
			if (rtn.valid()) {
                util::string ans("ans");
                variables.insert(ans, rtn->clone());
			}
		}		
		if (!rtn.valid()) {
			for (unsigned i = 0; i < base::input.get_pos(); ++i) {
				base::output << ' ';
			}
			base::output << "^\nCommand couldn't be evaluated. Terminated with error message:\n" 
            << rtn.get_info() << '\n';
		}
		else {
            base::output << *rtn;
		}
		return rtn;
	}
	
	var_evaluator(out_stream& output) : base(output), variables() {}
	
protected:
	
	var_map variables;

};
#endif
