#pragma once

#include "../given/tree_bag.hpp"
#include "../given/searchable_bag.hpp"

class searchable_tree_bag : public tree_bag, public searchable_bag
{
	public:
		searchable_tree_bag() {}
		searchable_tree_bag(const searchable_tree_bag& other) : tree_bag(other) {}
		searchable_tree_bag& operator=(const searchable_tree_bag& other);

		bool	has(int) const;
};
