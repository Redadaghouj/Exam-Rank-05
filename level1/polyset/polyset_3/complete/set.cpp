#include "set.hpp"

set& set::operator=(const set& other)
{
	if (&other != this)
		_bag = other._bag;
	return (*this);
}

bool	set::has(int val) const
{
	return (_bag->has(val));
}

void	set::insert(int val)
{
	if (!has(val))
		_bag->insert(val);
}

void	set::insert(int* arr, int size)
{
	for (int i=0; i<size; i++)
		insert(arr[i]);
}

void	set::print() const
{
	_bag->print();
}

void	set::clear()
{
	_bag->clear();
}

const searchable_bag& set::get_bag() const
{
	return (*_bag);
}
