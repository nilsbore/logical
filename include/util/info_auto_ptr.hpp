#ifndef INFO_AUTO_PTR_HPP
#define INFO_AUTO_PTR_HPP

namespace util {

	template <typename Matrix, typename Message>
	class info_auto_ptr {
	private:
		
		typedef info_auto_ptr self;
		typedef Matrix matrix_type;
		typedef Message message_type;
		
	public:
		
		enum ptr_type {
			VALID, VALID_VOID, NOT_FOUND, LOGIC_ERROR, SYNTAX_ERROR, NOT_INIT
		};
		
		self& operator+ (self& other)
		{
			if (dim_mismatch(other)) {
				*message = "Error in plus operation: operand dimension mismatch.";
				return *this;
			}
			if (other.matrix->is_dense()) {
				*other.matrix += *matrix;
				return other;
			}
			*matrix += *other.matrix;
			return *this;
		}
		
		self& operator- (self& other)
		{
			if (dim_mismatch(other)) {
				*message = "Error in minus operation: operand dimension mismatch.";
				return *this;
			}
			if (other.matrix->is_dense()) {
				*other.matrix -= *matrix;
				return other;
			}
			*matrix -= *other.matrix;
			return *this;
		}
		
		self& operator* (self& other)
		{
			if (dim_mismatch(other)) {
				*message = "Error in times operation: operand dimension mismatch.";
				return *this;
			}
			if (other.matrix->is_sparse()) {
				*other.matrix *= *matrix;
				return other;
			}
			*matrix *= *other.matrix;
			return *this;
		}
		
		self& operator/ (self& other)
		{
			if (dim_mismatch(other)) {
				*message = "Error in division operation: operand dimension mismatch.";
				return *this;
			}
			if (other.matrix->is_sparse()) {
				*other.matrix /= *matrix;
				return other;
			}
			*matrix /= *other.matrix;
			return *this;
		}
		
		bool dim_mismatch(const self& other)
		{
			if (matrix->is_scalar() || other.matrix->is_scalar()) {
				return false;
			}
			if (matrix->height() == other.matrix->height() &&
				matrix->width() == other.matrix->width()) {
				return false;
			}
			delete matrix;
			matrix = NULL;
			type = LOGIC_ERROR;
			if (message == NULL) {
				message = new util::string();
			}
			return true;
		}
		
		matrix_type& operator* () { return *matrix; }
		matrix_type* operator-> () { return matrix; }
		
		matrix_type* release()
		{
			matrix_type* rtn = matrix;
			matrix = NULL;
            delete message;
			message = NULL;
			type = NOT_INIT;
			return rtn;
		}

        // can be const since same class
        void operator= (const self& other)
		{
			if (matrix == other.matrix) {
				return;
			}
            delete matrix; delete message;
			matrix = other.matrix;
			message = other.message;
			type = other.type;
			other.matrix = NULL;
			other.message = NULL;
			other.type = NOT_INIT;
		}

        void operator= (self&& other)
        {
            if (matrix == other.matrix) {
                return;
            }
            delete matrix; delete message;
            matrix = other.matrix;
            message = other.message;
            type = other.type;
            other.matrix = NULL;
            other.message = NULL;
            other.type = NOT_INIT;
        }
		
		message_type get_info() const
		{
			if (message == NULL) {
				return message_type("No information supplied.");
			}
			return message_type(*message);
		}
		
		bool valid() const
		{
            return (matrix != NULL && type == VALID) || type == VALID_VOID;
		}
		
		info_auto_ptr() : matrix(NULL), message(NULL), type(NOT_INIT) {}
		info_auto_ptr(matrix_type* matrix) : matrix(matrix), message(NULL), type(VALID) {}
		info_auto_ptr(const char* str) : matrix(NULL), message(new message_type(str)), type(SYNTAX_ERROR) {}
		info_auto_ptr(ptr_type type) : matrix(NULL), message(NULL), type(type) {}
		info_auto_ptr(const self& other)
		{
			matrix = other.matrix;
			message = other.message;
			type = other.type;
			other.matrix = NULL;
			other.message = NULL;
			other.type = NOT_INIT;
		}
        info_auto_ptr(self&& other)
        {
            matrix = other.matrix;
            message = other.message;
            type = other.type;
            other.matrix = NULL;
            other.message = NULL;
            other.type = NOT_INIT;
        }
		
        ~info_auto_ptr() { delete matrix; delete message; }
		
	private:
		
		mutable matrix_type* matrix;
		mutable message_type* message;
		mutable ptr_type type;
		
	};
	
} // namespace util
#endif
