#pragma once

#include <ostream>
#include <string>

class bigint
{
	private:
// internals
		std::string	_s;

		bool	toUl(unsigned long& n) const;

	public:
// constructors + destructor
		bigint() : _s("0") {}
		bigint(unsigned long n);
		bigint(const bigint& o) : _s(o._s) {}
		bigint& operator=(const bigint& o);
		~bigint() {}

// addition
		bigint	operator+(const bigint& o) const;
		bigint&	operator++();
		bigint	operator++(int);
		bigint&	operator+=(const bigint& o);

// digits shift
		bigint	operator<<(int n) const;
		bigint&	operator<<=(int n);
		bigint	operator>>(int n) const;
		bigint&	operator>>=(int n);

		bigint	operator<<(const bigint& o) const;
		bigint&	operator<<=(const bigint& o);
		bigint	operator>>(const bigint& o) const;
		bigint&	operator>>=(const bigint& o);

// comparisons
		bool	operator<(const bigint& o) const;
		bool	operator<=(const bigint& o) const;
		bool	operator>(const bigint& o) const;
		bool	operator>=(const bigint& o) const;
		bool	operator==(const bigint& o) const;
		bool	operator!=(const bigint& o) const;

// cout <<
		friend std::ostream&	operator<<(std::ostream& oss, const bigint& o);
};
