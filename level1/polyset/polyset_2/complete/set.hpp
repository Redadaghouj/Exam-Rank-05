#pragma once

#include "../given/searchable_bag.hpp"

class set
{
	private:
		searchable_bag*	_bag;

	public:
		set();
		set(searchable_bag& bag);
		set(const set&);
		set& operator=(const set&);
		~set();

		void	insert(int);
		void	insert(int*, int);
		void	print() const;
		void	clear();

		bool	has(int) const;

		const searchable_bag&	get_bag() const;
};
