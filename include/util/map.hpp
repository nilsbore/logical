#ifndef MAP_HPP
#define MAP_HPP

namespace util {

	template <typename Key, typename Value>
	struct map_node {
		
		typedef map_node self;
		typedef Key key_type;
		typedef Value value_type;
		
		self* next_node;
		key_type key;
        value_type* val;
		
        map_node(key_type& nkey, value_type* nval) : key(nkey), val(nval),
													 next_node(NULL) {}
		
		~map_node()
		{
			delete next_node;
		}
		
	};
	
	template<typename Key, typename Value>
	class map {
	public:
		
		typedef map self;
		typedef Key key_type;
		typedef Value value_type;
		typedef map_node<Key, Value> node_type;
		
        bool insert(key_type& key, value_type* val) {
			unsigned pos = key.hash_code();
			pos %= capacity;
			node_type* current = vals[pos];
			if (current == NULL) {
				vals[pos] = new node_type(key, val);
				length++;
				return true;
			}
			while (true) {
				if (current->key == key) {
					current->val = val;
					return false;
				}
				else if (current->next_node == NULL) {
					current->next_node = new node_type(key, val);
					length++;
					return true;
				}
				current++;
			}
		}
		
        value_type* operator[] (const key_type& key) const
		{
			unsigned pos = key.hash_code();
			pos %= capacity;
			node_type* current = vals[pos];
			while (current != NULL) {
				if (current->key == key) {
                    return current->val; // ->clone()
				}
				current = current->next_node;
			}			
            return NULL;
		}
		
        value_type* pull(const key_type& key)
		{
			unsigned pos = key.hash_code();
			pos %= capacity;
			node_type* previous = vals[pos];
			node_type* current = vals[pos];
			while (current != NULL) {
				if (current->key == key) {
                    value_type* rtn = current->val;
                    previous->next_node = current->next_node;
                    current->next_node = NULL;
					delete current;
					length--;
					return rtn;
				}
				previous = current;
				current = current->next_node;
			}			
            return NULL;
		}
		
        /*value_type* get_pointer(const key_type& key)
		{
			unsigned pos = key.hash_code();
			pos %= capacity;
			node_type* current = vals[pos];
			while (current != NULL) {
				if (current->key == key) {
					return &(current->val);
				}
				current = current->next_node;
			}			
			return NULL;
        }*/
		
		unsigned size() { return length; }
		
		map(const unsigned n = 31) : length(0), capacity(n)
		{
			vals = new node_type*[n];
		}
		
		~map()
		{
			for (unsigned i = 0; i < capacity; ++i) {
				delete vals[i];
			}
			delete[] vals;
		}
		
	private:
		node_type** vals;
		unsigned length, capacity;
	};

} // namespace util
#endif
