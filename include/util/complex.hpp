#ifndef COMPLEX_HPP
#define COMPLEX_HPP

namespace util {
		
	template<typename Value>
	class complex {
	public:
		
		typedef complex self;
		typedef Value value_type;
		
		friend value_type re(const self& c) { return c.real; }
		friend value_type& re(self& c) { return c.real; }
		friend value_type im(const self& c) { return c.imag; }
		friend value_type& im(self& c) { return c.imag; }

        operator char() const { return char(real); }
        operator value_type() const { return char(real); }
        operator unsigned() const { return unsigned(real); }
		
		self operator* (const self& other) const
		{
			self rtn;
			re(rtn) = real * re(other) - imag * im(other);
            im(rtn) = real * im(other) + imag * re(other);
			return rtn;
		}
		
		self& operator*= (const self& other)
		{
			value_type tempreal = real;
			real *= re(other);
			real -= imag * im(other);
			imag *= re(other);
			imag += tempreal * im(other);
			return *this;
		}
		
		self operator/ (const self& other) const
		{
			value_type norm = re(other)*re(other) + im(other)*im(other);
			self rtn(real, imag);
			rtn *= self(re(other)/norm, -im(other)/norm);
			return rtn;
		}
		
		self operator/= (const self& other)
		{
			value_type norm = re(other)*re(other) + im(other)*im(other);
			*this *= self(re(other)/norm, -im(other)/norm);
			return *this;
		}
		
		self operator+ (const self& other) const
		{
			self rtn;
			re(rtn) = real + re(other);
			im(rtn) = imag + im(other);
			return rtn;
		}
		
		self& operator+= (const self& other)
		{
			real += re(other);
			imag += im(other);
			return *this;
		}
		
		self operator- (const self& other) const
		{
			self rtn;
			re(rtn) = real - re(other);
			im(rtn) = imag - im(other);
			return rtn;
		}
		
		self& operator-= (const self& other)
		{
			real -= re(other);
			imag -= im(other);
			return *this;
		}
		
		bool operator== (const self& other) const
		{
			return real == re(other) && imag == im(other);
		}

        bool operator!= (const self& other) const
        {
            return !this->operator ==(other);
        }
		
		self& operator= (const self& other)
		{
			real = re(other);
			imag = im(other);
			return *this;
		}
		
		complex() : real(value_type(0)), imag(value_type(0)) {}
        complex(value_type nre) : real(nre), imag(value_type(0)) {}
		
        complex(value_type nre, value_type nim) :
		real(nre), imag(nim) {}
		
		complex(const self& other) : real(re(other)), imag(im(other)) {}
		
	private:
		
        value_type real, imag;
		
	};
	
	template<typename OutStream, typename Value>
	OutStream& operator<< (OutStream& s, const complex<Value>& c)
	{
        if (re(c) == 0 && im(c) == 0) {
            s << 0;
        }
		if (re(c) != 0) {
			s << re(c);
			if (im(c) != 0) {
				s << '+';
			}
		}
		if (im(c) != 0) {
			if (im(c) != 1) {
				s << im(c);
			}
			s << 'i';
		}
		return s;
	}
	
} // namespace util

#endif
