#ifndef OPERATOR_PRIORITIES_HPP
#define	OPERATOR_PRIORITIES_HPP

namespace logic {
	
	class operator_priorities {
		
	public:
		
		static const unsigned nbr_of_levels = 5;
		
	private:
		
		static const unsigned level0_operators = 1;
		static const unsigned level1_operators = 1;
        static const unsigned level2_operators = 1;//2;
		static const unsigned level3_operators = 2;
		static const unsigned level4_operators = 1;

		static const char level0[level0_operators];
		static const char level1[level1_operators];
		static const char level2[level2_operators];
		static const char level3[level3_operators];
		static const char level4[level4_operators];

	public:
		
		static const char* operators[nbr_of_levels];
		static const unsigned nbr_of_operators[nbr_of_levels];
		
		static bool is_operator(char symbol)
		{
			for (unsigned i = 0; i < nbr_of_levels; ++i) {
				for (unsigned j = 0; j < nbr_of_operators[i]; ++j) {
					if (symbol == operators[i][j]) {
						return true;
					}
				}
			}
			return false;
		}
		
		static bool is_operator_with_priority(unsigned level, char symbol)
		{
			if (level >= nbr_of_levels) {
				return false;
			}
			for (unsigned i = 0; i < nbr_of_operators[level]; ++i) {
				if (symbol == operators[level][i]) {
					return true;
				}
			}
			return false;
		}
		
		static bool is_operator_with_priority_higher_than(unsigned level, char symbol)
		{
			if (level + 1 >= nbr_of_levels) {
				return false;
			}
			for (unsigned i = level + 1; i < nbr_of_levels; ++i) {
				for (unsigned j = 0; j < nbr_of_operators[i]; ++j) {
					if (symbol == operators[i][j]) {
						return true;
					}
				}
			}
			return false;
		}

	};
	
	const char operator_priorities::level0[] = {'\''};
	const char operator_priorities::level1[] = {'^'};
    const char operator_priorities::level2[] = {'*'};//, '/'};
	const char operator_priorities::level3[] = {'+', '-'};
	const char operator_priorities::level4[] = {':'};
	
	const char* operator_priorities::operators[] = {operator_priorities::level0,
													operator_priorities::level1, 
													operator_priorities::level2,
													operator_priorities::level3,
													operator_priorities::level4};

	
	const unsigned operator_priorities::nbr_of_operators[] = {operator_priorities::level0_operators,
															  operator_priorities::level1_operators,
															  operator_priorities::level2_operators,
															  operator_priorities::level3_operators,
															  operator_priorities::level4_operators};


} // namespace logic
#endif
