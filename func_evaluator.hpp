#ifndef FUNC_EVALUATOR_HPP
#define FUNC_EVALUATOR_HPP

#include "var_evaluator.hpp"
#include "util/file.hpp"

template<typename Value, typename OutStream>
class func_evaluator : public var_evaluator<Value, OutStream> {
protected:
	
	typedef func_evaluator self;
	typedef base_evaluator<Value, OutStream> base;
	typedef var_evaluator<Value, OutStream> var;
	typedef Value value_type;
	typedef OutStream out_stream;
	typedef typename var::matrix_type matrix_type;
	typedef typename var::return_type return_type;
	typedef util::map<util::string, util::file*> func_map; 
	typedef util::list<util::list<util::string> > namespace_stack;
	
	void enter_namespace()
	{
		last_vars.insert_empty();
	}
	
	void exit_namespace()
	{
		for ((*last_vars).begin(); !(*last_vars).end(); (*last_vars)++) {
			var::variables.pull(*(*last_vars));
		}
		last_vars.delete_last();
	}
	
	return_type function(const util::string& funcname, util::list<matrix_type>& args) {
		return_type temp = base::function(funcname);
		if (temp.valid()) return temp;
		util::file* f = func_map.get_pointer(funcname);
		if (f == NULL) return "No match for function.";
		self func_eval(base::output);
		return func_eval.evaluate_function(f, funcname, args);		
	}
	
	return_type evaluate_function(util::file* func, util::string& funcname, util::list<matrix_type>& args)
	{
		last_line = false;
		util::string checkfunc = base::input.next_word();
		if (checkfunc != "function") return "Function definition has to begin with function.";
		base::input.next_blanks();
		util::string rtnname;
		base::input.next_blanks();
		if (*base::input != '=') return "Function has to return value with = operator";
		base::input++;
		base::input.next_blanks();
		checkfunc = base::input.next_word();
		if (checkfunc != funcname) return "Function has to have same name as file.";
		base::input.next_blanks();
		if (*base::input != '(') return "Function has to take at least one parameter.";
		base::input++;
		for (args.begin(); args++; !args.end()) {
			util::string varname = base::input.next_word();
			if (varname.is_empty()) return "Invalid argument name.";
			var::variables.insert(varname, *args);
			base::input.next_blanks();
			if (*base::input == ')') {
				break;
			}
			if (*base::input != ',') return "Parameters are not separated by commas.";
			base::input++;
			base::input.next_blanks();
		}
		if (!base::input.end()) return "Too few parameters supplied.";
		//set_script(true); // perhaps not needed?
		return_type temp = execute_statement(false);
		//set_script(false); // perhaps not needed?
		if (!temp.valid()) return temp;
		matrix_type rtn = var::variables.pull(rtnname);
		if (rtn.is_empty()) return "Return value hasn't been assigned.";
		return rtn;
	}
	
	return_type skip_statement(bool is_if)
	{
		return_type temp;
		while (true) {
			util::string word = base::input.get_name();
			if (!word.is_empty()) {
				if (word == "end") {
					return return_type::VALID_VOID;
				}
				if (word == "if") {
					temp = next_line();
					if (!temp.valid()) return temp;
					return skip_statement(true);
				}
				else if (word == "for" || word == "while") {
					temp = next_line();
					if (!temp.valid()) return temp;
					return skip_statement(false);
				}
				else if (!is_if && word == "else") return "Else only in combination with if statements.";
			}
			temp = next_line();
			if (!temp.valid()) return temp;
		}
	}
	
	return_type execute_while()
	{
		util::file::pos start = function->get_pos();
		util::string statement = base::input.remaining_string();
		return_type res = evaluate_statement(statement);
		if (!res.valid()) return res;
		return_type temp;
		if (res.value) {
			enter_namespace();
			do {
				function->set_pos(start);
				temp = execute_statement(false);
				if (!temp.valid()) return temp;
				res = evaluate_statement(statement);
				if (!res.valid()) return res; 
			}
			while (res.value);
			exit_namespace();
			return;
		}
		next_line();
		return skip_statement(false);
	}
	
	return_type execute_for()
	{
		util::file::pos start = function->get_pos();
		base::input.next_blanks();
		util::string iter = base::input.next_word();
		if (iter.is_empty()) return "No name assigned for iterator.";
		if (iter == "i") return "Can't assign other value to constant i;";
		base::input.next_blanks();
		if (base::input != '=') return "No assignment in for statement.";
		base::input++;
		return_type loop = evaluate_statement();
		if (!loop.valid()) return loop;
		return_type temp;
		enter_namespace();
		for (unsigned row = 0; row < loop.value.height(); ++row) {
			for (unsigned col = 0; col < loop.value.width(); ++col) {
				function->set_pos(start);
				var::variables.insert(iter, matrix_type::real_scalar(loop.value(row, col)));
				temp = execute_statement(false);
				if (!temp.valid()) return temp;
			}
		}
		exit_namespace();
		return return_type::VALID_VOID;
	}
	
	return_type execute_if()
	{
		return_type run = evaluate_statement();
		if (!run.valid()) return run;
		return_type rtn;
		if (run.value) {
			enter_namespace();
			rtn = execute_statement(true);
			exit_namespace();
			return rtn;
		}
		while (true) {
			util::string word = base::input.next_word();
			if (!word.is_empty()) {
				if (word == "end") {
					return return_type::VALID_VOID;
				}
				if (word == "else") {
					if (!base::input.end() && *base::input == ' ') {
						base::input++;
						if (!base::input.end()) {
							util::string word = base::input.next_word();
							if (!word.is_empty()) {
								if (word == "if") {
									return execute_if();
								}
							}
						}
					}
					enter_namespace();
					rtn = execute_statement(false);
					exit_namespace();
					return rtn;
				}
			}
			next_line();
		}
	}
	
	void add_to_namespace(util::string& varname)
	{
		(*last_vars).insert(varname);
	}
	
	return_type evaluate_statement()
	{
		return base::build_matrix(';', '#', true);
	}
	
	return_type evaluate_statement(util::string& input)
	{
		base::input.new_expression(input);
		return base::build_matrix(';', '#', true);
	}
	
	return_type evaluate_line()
	{
		if (base::input.contains_def()) {
			return var::assignment();
		}
		return base::build_matrix(';', ';', true);
	}
	
	return_type execute_statement(bool is_if)
	{
		unsigned pos;
		return_type temp;
		while (true) {
			temp = next_line();
			if (!temp.valid()) return temp;
			pos = base::input.get_pos();
			util::string word = base::input.next_word();
			if (!word.is_empty()) {
				if (word == "end") {
					return return_type::VALID_VOID;
				}
				if (word == "if") {
					temp = execute_if();
					if (!temp.valid()) return temp;
					continue;
				}
				if (word == "for") {
					temp = execute_for();
					if (!temp.valid()) return temp;
					continue;
				}
				if (word == "while") {
					temp = execute_while();
					if (!temp.valid()) return temp;;
					continue;
				}
				if (word == "else") {
					if (!is_if) {
						return "Else only in combination with if statements.";
					}
					temp = next_line();
					if (!temp.valid()) return temp;
					return skip_statement(true);
				}
				if (word == "exit") {
					return "Function stack quit with command 'exit'.";
				}
				base::input.set_pos(pos);
			}
			return_type result = evaluate_line(); // IMPLEMENTERA
			if (!result.valid()) return result;
		}
	}
	
	return_type next_line()
	{
		(*function)++;
		if (function->end()) return "End of file reached while evaluating function.";
		base::input.new_expression(function->get_line(), function->get_length());
		return return_type::VALID_VOID;
	}
	
public:
	
	func_evaluator(out_stream& output) : var(output), last_vars() {}
	
private:
	
	util::file* function;
	func_map functions;
	namespace_stack last_vars;
	bool last_line;
};
#endif