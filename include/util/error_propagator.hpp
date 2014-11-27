#ifndef ERROR_PROPAGATOR_HPP
#define ERROR_PROPAGATOR_HPP

namespace util {

	template<typename Value, typename Message>
	struct error_propagator {
	private:
		
		typedef error_propagator self;
		typedef Value value_type;
		typedef Message message_type;
		
		enum propagator_type {
			VALID, VALID_VOID, NOT_FOUND, USER_ERROR, SYNTAX_ERROR, NOT_INIT 
		} type;
		
		mutable value_type* value;
		mutable message_type* message;
		
	public:
		
		bool valid() const
		{
			return type == VALID || type == VALID_VOID;
		}
		
		value_type& operator* () { return *value; }
		
		self& operator= (const self& other)
		{
			delete value;
			delete message;
			value = other.value;
			message = other.message;
			other.value = NULL;
			other.message = NULL;
			type = other.type;
		}
		
		self& operator= (const char* nmessage)
		{
			delete value;
			delete message;
			value = NULL;
			message = new message_type(nmessage);
			type = SYNTAX_ERROR;
		}
		
		self& operator= (const value_type& nvalue)
		{
			delete value;
			delete message;
			value = new value_type(nvalue);
			message = NULL;
		}
		
		error_propagator() : value(NULL), message(NULL), type(NOT_INIT) {}
		
		error_propagator(propagator_type type) : value(NULL), message(NULL), type(type) {}
		
		error_propagator(const self& other)
		{
			value = other.value;
			message = other.message;
			other.value = NULL;
			other.message = NULL;
			type = other.type;
			other.type = NOT_INIT;
		}
		
		error_propagator(const char* nmessage) : value(NULL), type(SYNTAX_ERROR)
		{
			message = new message_type(nmessage);
		}
		
		error_propagator(const value_type& nvalue) : value(nvalue), message()
		{
			type = value.is_empty()? INPUT_ERROR : VALID;
			if (type == INPUT_ERROR) {
				message = new message_type("Empty matrix returned at some point.");
			}
		}
		
	};
	
	template<typename Value, typename Message>
	error_propagator<Value, Message>  operator* (error_propagator<Value, Message>& first,
												 error_propagator<Value, Message>& second)
	{
		typedef error_propagator<Value, Message> return_type;
		if (first.is_scalar()) {
			*second *= *first;
			return_type rtn(second);
			return rtn;
		}
		else if (second.is_scalar()) {
			*first *= *second;
			return_type rtn(first);
			return rtn;
		}
		if (dim_mismatch(first, second)) return "Dimensions don't agree.";
		if (first.is_sparse()) {
			*first *= *second;
			return_type rtn(first);
			return rtn;
		}
		else if (second.is_sparse()) {
			*second *= *first;
			return_type rtn(second);
			return rtn;
		}
		else {
			*first *= *second;
			return_type rtn(first);
			return rtn;
		}
	}
	
} // namespace util
#endif