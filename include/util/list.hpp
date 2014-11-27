#ifndef LIST_HPP
#define LIST_HPP

namespace util {

	template<typename Value>
	class list {
	public:
		
		typedef list self;
		typedef Value value_type;
		
		struct node {
			
			node(value_type* value) : value(value), next(NULL) {}
            ~node() { delete next; delete value; }
			
			node* next;
			node* previous;
			value_type* value;
			
		};
		
		node* get_pos() { return pos; }		
		void set_pos(node* newpos) { pos = newpos; }
		
		void begin() { pos = first; }
		bool end() { return pos == NULL; }
		
		void operator++ (int) { pos = pos->next; }
		
		value_type& operator* ()
		{
			/*if (pos == NULL) {
				return value_type();
			}*/
			return *(pos->value);
		}
		
		value_type* get_pointer() // TODO
		{
			if (pos == NULL) {
				return NULL;
			}
			return pos->value;
		}
		
		value_type* release() // take close look
		{
			if (pos == NULL) {
				return NULL;
			}
			if (pos == first) {
				first = pos->next;
				if (pos == last) {
					last = NULL;
				}
				else {
					first->previous = NULL;
				}
			}
			else {
				pos->previous->next = pos->next;
			}
			
			if (pos == last) {
				last = pos->previous;
				if (pos == first) {
					first = NULL;
				}
				else {
					last->next = NULL;
				}
			}
			else {
				pos->next->previous = pos->previous;
			}
			
			value_type* rtn = pos->value;
			pos->next = NULL;
			pos->previous = NULL;
			pos->value = NULL;
			delete pos;
			pos = first;
			return rtn;
		}
		
		void insert(value_type* value)
		{
			if (first == NULL) {
				first = new node(value);
				last = pos = first;
				size++;
				return;
			}
			last->next = new node(value);
			last->next->previous = last;
			last = last->next;
			size++;
		}
		
		void insert_empty()
		{
			insert(new value_type());
			size++;
		}
		
		void delete_last()
		{
			if (last == NULL) {
				return;
			}
			if (last->previous != NULL) {
                last->previous->next = NULL;
			}
			if (first == last) {
				first = NULL;
			}
			if (pos == last) {
				pos = first;
			}
			delete last;
			last = NULL;
			size--;
		}
		
		value_type* find(value_type& value)
		{
			node* current = first;
			while (current != NULL) {
				if (*(current->value) == value) {
					return current->value;
				}
				current = current->next;
			}
			return NULL;
		}
		
		unsigned length() { return size; }
		
		list() : first(NULL), last(NULL), pos(NULL), size(0) {}
		~list() { delete first; }
		
	private:
		
		node* first;
		node* last;
		node* pos;
		unsigned size;
	};
	
} // namespace util
#endif
