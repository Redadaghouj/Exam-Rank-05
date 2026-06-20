#include "searchable_tree_bag.hpp"

searchable_tree_bag& searchable_tree_bag::operator=(const searchable_tree_bag& o)
{
	if (this != &o)
		tree_bag::operator=(o);
	return (*this);
}

bool	searchable_tree_bag::has(int val) const
{
	node* tmp = tree;

	while (tmp)
	{
		if (tmp->value == val) return (true);
		else if (tmp->value > val) tmp = tmp->l;
		else tmp = tmp->r;
	}
	return (false);
}