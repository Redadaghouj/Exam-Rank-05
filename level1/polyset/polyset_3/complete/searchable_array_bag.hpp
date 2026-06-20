#include "../given/array_bag.hpp"
#include "../given/searchable_bag.hpp"

class searchable_array_bag : public searchable_bag, public array_bag
{
	public:

		searchable_array_bag() {}
		searchable_array_bag(const searchable_array_bag& o) : array_bag(o) {}
		searchable_array_bag& operator=(const searchable_array_bag&);
		~searchable_array_bag() {}

		bool	has(int) const;
};
