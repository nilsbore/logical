#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

namespace logic {
	
	template<typename Value>
	struct plus {
		Value operator() (Value first, Value second)
		{
			return first + second;
		}
	};

	template<typename Value>
	struct minus {
		Value operator() (Value first, Value second)
		{
			return first - second;
		}
	};

	template<typename Value>
	struct times {
		Value operator() (Value first, Value second)
		{
			return first * second;
		}
	};

	template<typename Value>
	struct through {
		Value operator() (Value first, Value second)
		{
			return first / second;
		}
	};
	
} // namespace logic
#endif