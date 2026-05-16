#include "searchable_tree_bag.hpp"

searchable_tree_bag& searchable_tree_bag::operator=(const searchable_tree_bag& other)
{
	if (this != &other)
		tree_bag::operator=(other);
	return (*this);
}

bool	searchable_tree_bag::has(int val) const
{
	node*	tmp;

	tmp = tree;
	while (tmp)
	{
		if (tmp->value == val) return (true);
		else if (tmp->value < val) tmp = tmp->l;
		else tmp = tmp->r;
	}
	return (false);
}
