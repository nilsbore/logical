#ifndef FILE_HPP
#define FILE_HPP

#include "list.hpp"
#include "string.hpp"
#include <fstream>

namespace util {
	
	class file {
	public:
		typedef file self;
        typedef list<unsigned>::node* line_pos;
        typedef list<char*>::node* data_pos;
		
		struct pos {
			line_pos length;
			data_pos line;
		};
		
        friend bool read_file(self&, const util::string&);

        static bool file_exists(util::string& name)
        {
            std::ifstream f(name.c_str());
            return f.good();
        }
				
		pos get_pos()
		{
			pos rtn;
			rtn.length = line_length.get_pos();
			rtn.line = data.get_pos();
			return rtn;
		}
		
		void set_pos(pos newpos)
		{
			line_length.set_pos(newpos.length);
			data.set_pos(newpos.line);
		}
		
		char* get_line()
		{
			if (data.end()) {
				return NULL;
			}
			return *data;
		}
		
		void operator++ (int)
		{

			if (!data.end() && !line_length.end()) {
				data++;
				line_length++;
			}
		}
		
		void begin()
		{
			data.begin();
			line_length.begin();
		}
		
		bool end()
		{
			return data.end();
		}
		
		unsigned get_length()
		{
			if (line_length.end()) {
				return 0;
			}
			return *line_length;
		}
		
		void insert_line(char* value, unsigned length)
		{
			if (value == NULL || length == 0) {
				return;
			}
            data.insert(new (char*)(value));
            line_length.insert(new unsigned(length));
		}
		
        file(const util::string& name) : data(), line_length()
		{
			read_file(*this, name);
		}
		
		~file()
		{
			for (data.begin(); !data.end(); data++) {
				delete[] *data;
			}
		}
		
	private:
		util::list<unsigned> line_length;
        util::list<char*> data;
	};
	
	template<typename OutStream>
	OutStream& operator<< (OutStream& s, file& f)
	{
		for (f.begin(); !f.end(); f++) {
			char* line = f.get_line();
			unsigned length = f.get_length();
			for (unsigned c = 0; c < length; ++c) {
				s << line[c];
			}
			s << '\n';
		}
		return s;
	}
	
    bool read_file(file& f, const util::string& name)
    {
		char* cstr = new char[name.length() + 4];
		unsigned i;
		for (i = 0; i < name.length(); ++i) {
			cstr[i] = name[i];
		}
		cstr[i] = '.'; cstr[i+1] = 'm'; cstr[i+2] = 'v'; cstr[i+3] = '\0';
		std::ifstream newfile;
		newfile.open(cstr);
		delete cstr;
        if (!newfile.is_open()) {
			return false;
		}
		std::string line;
		std::string::iterator c;
		unsigned start, length;
		while (!newfile.eof()) {
            std::getline(newfile, line);
			start = 0;
			for (c = line.begin(); c != line.end(); ++c, ++start) {
				if (*c != ' ' && *c != '\t') {
					break;
				}
			}
            if (c == line.end() || *c == '#' || *c == '\n') {
				continue;
			}
			length = line.size() - start;
			char* nline = new char[length];
            for (i = 0; i < length; ++i, ++c) {
				nline[i] = *c;
			}
			f.insert_line(nline, length);
		}
		newfile.close();

		return true;
	}

}
#endif
