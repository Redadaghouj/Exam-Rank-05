#include "../given/searchable_bag.hpp"

class set
{
	private:
		searchable_bag* _bag;

	public:
		set() {}
		set(const set& o) : _bag(o._bag) {}
		set(searchable_bag& bag) : _bag(&bag) {}
		set& operator=(const set& o);
		~set() {}

		void insert(int);
		void insert(int *, int);
		void print() const;
		void clear();

		bool	has(int) const;

		const searchable_bag&	get_bag() const;
};
