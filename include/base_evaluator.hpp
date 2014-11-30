#ifndef BASE_EVALUATOR_HPP
#define BASE_EVALUATOR_HPP

#include "expression.hpp"
#include "logic/dense_matrix.hpp"
#include "logic/sparse_matrix.hpp"
#include "logic/scalar.hpp"
#include "logic/string_matrix.hpp"
#include "logic/matrix_functions.hpp"
#include "logic/operator_priorities.hpp"
#include "logic/build_proxy.hpp"
#include "util/string.hpp"
#include "util/info_auto_ptr.hpp"

//#include <iostream> // FOR DEBUG

using namespace logic;

template <typename Value, typename OutStream>
class base_evaluator {    
protected:
	
	typedef OutStream out_stream;
	typedef Value value_type;
	typedef virtual_matrix<value_type> matrix_type;
    typedef typename matrix_type::dense_type dense_type;
    typedef typename matrix_type::sparse_type sparse_type;
    typedef typename matrix_type::string_type string_type;
	typedef scalar<value_type> scalar_type;
	typedef expression<typename util::get_base_type<value_type>::type> expression_type;
    typedef build_proxy<matrix_type> build_type;

public:

    typedef util::info_auto_ptr<matrix_type, util::string> return_type;

protected:
	
	return_type get_scalar()
	{
		return new scalar_type(input.next_float());
	}
	
	return_type text()
	{
		return_type rtn(new string_type(input.next_string()));
		if (*input != '"') return "String not terminated correctly.";
		input++;
		return rtn;
	}
	
	return_type arguments(util::list<matrix_type>& goal)
	{
		if (*input == ')') {
			return return_type::VALID_VOID;
		}
		return_type arg;
		while (!input.end()) {
			arg = build_matrix(',', ')', false);
			if (!arg.valid()) return arg;
			goal.insert(arg.release());
			if (*input == ')') {
				break;
			}
			input++;
		}
		if (*input != ')') return "End reached while evaluating function argument.";
		input++;
		return return_type::VALID_VOID;
	}
	
	virtual return_type function(const util::string& arg)
	{
		util::list<matrix_type> args;
		return_type temp = arguments(args);
		if (!temp.valid()) return temp;
		return base_functions(arg, args);;
	}
	
	virtual return_type name(const util::string& arg) {
		if (*input == '(') {
			input++;
			return function(arg);
		}
		if (arg == "pi") {
			return new scalar_type(3.14159265);
		}
		else if (arg == "e") {
			return new scalar_type(2.71828183);
		}
		else if (arg == "author") {
			return new string_type(util::string("Nils Bore"));
		}
		else if (arg == "end") {
			return new scalar_type(index_end);
		}
		return "Variable not found."; // fix showing name
	}
	
	return_type build_index(const matrix_type& value)
	{
		unsigned previousend = index_end;
		if (value.is_vector()) {
			index_end = value.length() - 1;
		}
		else {
			index_end = value.height() - 1;
		}
		return_type first = build_matrix(',', ']', false);
		if (!first.valid()) return first;
		
		if (*input == ']') {
			input++;
			index_end = previousend;
			return index(value, *first);
		}
		if (*input != ',') return "Missing ',' in index argument.";
		input++;
		input.next_blanks();
		index_end = value.width() - 1;
		return_type second = build_matrix(']', ']', false);
		if (!second.valid()) return second;
		if (*input != ']') return "Index expression not terminated correctly.";
		input++;
		index_end = previousend;
		return logic::index(value, *first, *second);
	}
	
	return_type decide_type() // fixa plugin för användar-definierade typer. Metod i Object
	{
		bool positive = true;
		if (*input == '+') {
			positive = true;
			input++;
			input.next_blanks();
		}
		else if (*input == '-') {
			positive = false;
			input++;
			input.next_blanks();
		}
		return_type rtn;
		if (*input == '(') {
			input++;
			rtn = build_matrix(')');
		}
		else if (input.is_number()) {
			rtn = get_scalar();
		}
		else if (input.is_letter()) {
			rtn = name(input.next_name());
		}
		else if (*input == '"') {
			input++;
			rtn = text();
		}
		else {
			return "Symbol not recognized as type.";
		}
		if (!rtn.valid()) return rtn;
		if (*input == '[') {
			input++; // skipblanks
			rtn = build_index(*rtn);
			if (!rtn.valid()) return rtn;
		}
		if (!positive) {
			*rtn *= scalar_type(-1);
		}
		return rtn;
	}
	
	return_type build_matrix(char stop1, char stop2, bool base)
	{
		input.next_blanks();
		return_type temp = priority_level(operator_priorities::nbr_of_levels - 1, stop1, stop2);
		if (!temp.valid()) return temp;
		build_type rtn(temp);
		while (true) {
			input.next_blanks();
            if (*input == '\\') {
				input++;
				input.next_blanks();
				temp = priority_level(operator_priorities::nbr_of_levels - 1, stop1, stop2);
				if (!temp.valid()) return temp;
                if(!rtn.add_to_bottom(temp)) return "Matrix widths don't agree.";
			}
			else if (*input == stop1 || *input == stop2) {
				break;
			}
			else if (input.end()) {
				if (!base) return "Expression not terminated correctly.";
				break;
			}
			else {
				temp = priority_level(operator_priorities::nbr_of_levels - 1, stop1, stop2);
				if (!temp.valid()) return temp;
				if(!rtn.add_to_right(temp)) return "Matrix dimensions don't agree.";
			}
		}
		return rtn.build();
	}
	
	return_type build_matrix(char stop)
	{
		return_type rtn = build_matrix(stop, stop, false);
		// assert(*input == stop);
		input++;
		return rtn;
	}
	
	return_type build_linspace(const matrix_type& first, const matrix_type& second,
							   int level, char stop1, char stop2)
	{
		if (*input == ':') {
			input++;
			input.next_blanks();
			return_type third = priority_level(level - 1, stop1, stop2);
			if (!third.valid()) return third;
			return linspace(first, second, *third);
		}
		return linspace(first, second);
	}
	
	return_type invoke_operator(char symbol, return_type first, return_type second,
								int level, char stop1, char stop2)
	{
		// ändra för att ta return_type som argument istället

		return_type rtn;
		switch (symbol) {
			case '+':
				rtn = first + second;
				break;
			case '-':
				rtn = first - second;
				break;
			case '*':
				rtn = first * second;
				break;
            case '/':
                rtn = first / second;
                break;
			/*case '^':
				if (!second.is_scalar()) return "Exponent has to be scalar.";
				first = first ^ second;
				rtn = first;
				break;*/
			case ':':
				rtn = build_linspace(*first, *second, level, stop1, stop2);
				break;
			default:
				rtn = "Couldn't find operator.";
        }
		return rtn;
	}
	
	return_type priority_level0(char stop1, char stop2)
	{
		return_type rtn = decide_type();
		if (!rtn.valid()) return rtn;
		
		while (!input.end() && *input != stop1 && *input != stop2 && *input != '\\' &&
			   !operator_priorities::is_operator_with_priority_higher_than(0, *input)) {
			
			bool found = false;
			if (*input == ' ') {
				input.next_blanks();
				if (operator_priorities::is_operator_with_priority(0, *input)) {
					found = true;
				}
				else if (operator_priorities::is_operator_with_priority_higher_than(0, *input)) {
					break;
				}
				else {
					input.previous();
					break;
				}
			}
			
			if (found || operator_priorities::is_operator_with_priority(0, *input)) {
				rtn->transp();
				input++;
			}
			else {
				return "Couldn't identify highest priority operator.";
			}
			
		}
		
		return rtn;
	}
	
	return_type priority_level(int level, char stop1, char stop2)
	{
		if (level == 0) { 
			return priority_level0(stop1, stop2);
		}
		
		return_type rtn = priority_level(level - 1, stop1, stop2);
		if (!rtn.valid()) return rtn;
		
		while (!input.end() && *input != stop1 && *input != stop2 && *input != ' ' && *input != '\\' &&
			   !operator_priorities::is_operator_with_priority_higher_than(level, *input)) {
			
			if (operator_priorities::is_operator_with_priority(level, *input)) {
				
				if (level == 2 && input[-1] == ' ' && input[1] != ' ') {
					// level of + and -
					input.previous();
					break;
				}
				
				char symbol = *input;
				input++;
				input.next_blanks();
                return_type tmp = priority_level(level - 1, stop1, stop2);
				if (!tmp.valid()) return tmp;
                rtn = invoke_operator(symbol, rtn, tmp, level, stop1, stop2);
				if (!rtn.valid()) return rtn;
			}
			else {
				return "Couldn't identify operator.";
			}
			
        }

		return rtn;
	}
	
public:
	
	return_type evaluate(const char* arg, int length)
	{
		input.new_expression(arg, length);
        return_type rtn = build_matrix(';', ';', true);
		
		if (!rtn.valid()) {
            for (unsigned i = 0; i < 3+input.get_pos(); ++i) {
				output << ' ';
			}
			output << "^\nCommand couldn't be evaluated. Terminated with error message:\n" 
				   << rtn.get_info() << '\n';
		}
		else {
			output << *rtn;
		}
		return rtn;
	}
	
	base_evaluator(out_stream& output) : output(output), input(), index_end(0) {}
	
protected:
	
	const static bool is_evaluator = true;
	
	out_stream& output;
	expression_type input;
	unsigned index_end;

};
#endif
