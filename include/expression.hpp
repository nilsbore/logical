#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "util/string.hpp"

template <typename FloatType>
class expression {
public:
	
	typedef FloatType float_type;
	
	expression() : string(NULL), size(0), pos(0) {}
	
	expression(const char* string, unsigned size) : string(string),
													size(size), pos(0) {}
	
	void new_expression(const char* newstring, unsigned newsize)
	{
		size = newsize;
		pos = 0;
		string = newstring;
	}
	
	void new_expression(util::string& newstring)
	{
		new_expression(newstring.c_str(), newstring.length());
	}
	
	bool end()
	{
		return pos >= size;
	}
	
	char val() { return end()? ';' : string[pos]; }
	
	char operator* () { return val(); }
	
	unsigned get_pos() const { return pos; }
	void set_pos(unsigned newpos)
	{
		// assert(newpos >= 0 && newpos < size);
		pos = newpos;
	}
	
	bool is_number()
	{
		return !end() && val() >= '0' && val() <= '9';
	}
	
	bool is_letter()
	{
		return !end() && ((val() >= 'a' && val() <= 'z') || (val() >= 'A' && val() <= 'Z'));
	}
	
	bool contains_def()
	{
		for (unsigned i = 1; i < size - 1; i++) {
			if (string[i] == '=') {
				char previous = string[i - 1];
				if (previous == '!' || previous == '<' || previous == '>') {
					return false; // could be in index, fix
				}
				else if (string[i + 1] == '=') {
					return false;
				}
				return true;
			}
		}
		return false;
	}
	
	float_type next_float()
	{
		float_type sum = 0;
		while (is_number()) {
			sum = 10 * sum + (val() - '0');
			next();
		}
		if (val() != '.') {
			return sum;
		}
		next();
		double factor = 0.1;
		while (is_number()) {
			sum += factor * (val() - '0');
			next();
			factor *= 0.1;
		}
		return sum;
	}
	
	util::string next_name()
	{
		unsigned first = pos;
		while (!end() && is_letter()) {
			pos++;
		}
		util::string rtn(pos - first);
		for (unsigned i = first; i < pos; ++i) {
			rtn[i - first] = string[i];
		}
		return rtn;
	}
	
	util::string next_string()
	{
		unsigned first = pos;
		while (!end() && val() != '"') {
			pos++;
		}
		util::string rtn(pos - first);
		for (unsigned i = first; i < pos; ++i) {
			rtn[i - first] = string[i];
		}
		return rtn;
	}
	
	util::string remaining_string()
	{
		util::string rtn(size-pos);
		for (unsigned i = pos; i < size; ++i) {
			rtn[i-pos] = string[i];
		}
		return rtn;
	}
	
	bool next()
	{
		pos++;
		return !end();
	}
	
	bool previous()
	{
		if (pos == 0) {
			return false;
		}
		pos--;
		return true;
	}
	
	char operator[] (int rel) const
	{
		int offpos = pos + rel;
		if (offpos < 0 || offpos >= size) {
			return ';';
		}
		return string[offpos];
	}
	
	char operator++ (int)
	{
		pos++;
		return val();
	}
	
	bool next_blanks() {
		while (!end() && (val() == ' ' || val() == '\t')) {
			next();
		}
		return !end();
	}

    unsigned length() const { return size; }
	
private:
	
	const char* string;
	unsigned size, pos;
	
};

template<typename OutStream, typename FloatType>
OutStream& operator<< (OutStream& s, expression<FloatType>& str)
{
    unsigned pos = str.get_pos();
    str.set_pos(0);
    for (unsigned i = 0; i < str.length(); ++i) {
        s << str[i];
    }
    str.set_pos(pos);
    return s;
}
#endif
