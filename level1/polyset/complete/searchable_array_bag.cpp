#include "searchable_array_bag.hpp"

searchable_array_bag& searchable_array_bag::operator=(const searchable_array_bag& o)
{
	if (this != &o)
		array_bag::operator=(o);
	return (*this);
}

bool	searchable_array_bag::has(int val) const
{
	for (int i=0; i<size; i++)
		if (data[i] == val) return (true);
	return (false);
}