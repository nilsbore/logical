#ifndef STRING_HPP
#define STRING_HPP

namespace util {

	class string {
	private:
		
		typedef string self;
		
	public:
		
		string(const char* newvals, unsigned size) : size(size), vals(new char[size])
		{
			for (unsigned i = 0; i < size; ++i) {
				vals[i] = newvals[i];
			}
		}
		
		string(const char* newvals) : size(0)
		{
			while (newvals[size] != '\0') {
				size++;
			}
			vals = new char[size];
			for (unsigned i = 0; i < size; ++i) {
				vals[i] = newvals[i];
			}
		}
		
		string(const self& other) : size(other.size), vals(new char[other.size])
		{
			for (unsigned i = 0; i < size; ++i) {
				vals[i] = other[i];
			}
		}
		
		string(unsigned size) : size(size), vals(new char[size]) {}
		
		string() : size(0), vals(NULL) {}
		
		bool is_empty() const { return size == 0; }
		
		char* c_str() { return vals; }
		
		void operator= (const self& other)
		{
			delete[] vals;
			size = other.size;
			vals = new char[size];
			for (unsigned i = 0; i < size; ++i) {
				vals[i] = other[i];
			}
		}
		
		void operator= (const char* newvals)
		{
			delete[] vals;
			size = 0;
			while (newvals[size] != '\0') {
				size++;
			}
			vals = new char[size];
			for (unsigned i = 0; i < size; ++i) {
				vals[i] = newvals[i];
			}
		}
		
		~string()
		{
			delete[] vals;
		}
		
		char operator[] (unsigned pos) const
		{
			if (pos >= size || pos < 0) {
				return '\0';
			}
			return vals[pos];
		}
		
		char& operator[] (unsigned pos)
		{
			return vals[pos];
		}
		
		unsigned length() const
		{
			return size;
		}
		
		bool operator== (const self& other) const
		{
			if (length() != other.length()) {
				return false;
			}
			for (unsigned i = 0; i < size; i++) {
				if (vals[i] != other[i]) {
					return false;
				}
			}
			return true;
		}
		
		bool operator!= (const self& other) const
		{
			return !(*this == other);
		}
		
		bool operator== (const char* other) const
		{
			if (other == NULL || other == '\0') {
				return false;
			}
			for (unsigned i = 0; i < size; ++i) {
				if (vals[i] != *other || *other == '\0') {
					return false;
				}
				other++;
			}
			if (*other != '\0') {
				return false;
			}
			return true;
		}
		
		bool operator!= (const char* other) const
		{
			return !(*this == other);
		}
		
		unsigned hash_code() const {
			unsigned rtn = 1;
			for (unsigned i = 0; i < size; ++i) {
				rtn *= vals[i];
			}
			return rtn;
		}
		
	private:
		
		unsigned size;
		char* vals;
	};
	
	template<typename OutStream>
	OutStream& operator<< (OutStream& s, const string& str)
	{
		for (unsigned i = 0; i < str.length(); ++i) {
			s << str[i];
		}
		return s;
	}
		   
} // namespace util
#endif