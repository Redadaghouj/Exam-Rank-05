#pragma once

#include "../given/searchable_bag.hpp"
class set
{
	private:
		searchable_bag*	_bag;

	public:
		set() {};
		set(const set& other) : _bag(other._bag) {}
		set(searchable_bag& other) : _bag(&other) {}
		set& operator=(const set& other);
		~set() {}

		bool has(int) const;

		void	insert(int);
		void	insert(int *, int);
		void	print() const;
		void	clear();

		const searchable_bag& get_bag() const;
};
