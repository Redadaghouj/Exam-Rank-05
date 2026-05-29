#pragma once

#include <string>

class bigint
{
	private:
// internal
		std::string	_s;

		// void	trim();
		bool	toUl(unsigned long& out) const;

	public:
// constructors + destructor
		bigint() : _s("0") {};
		bigint(unsigned long n);
		bigint(const bigint& o) : _s(o._s) {};
		bigint&	operator=(const bigint& o);
		~bigint() {};

// addition
		bigint	operator+(const bigint& o) const;
		bigint&	operator+=(const bigint& o);
		bigint&	operator++();
		bigint	operator++(int);

// digits shift
		bigint	operator<<(int n) const;
		bigint&	operator<<=(int n);
		bigint	operator<<(const bigint& o) const;
		bigint&	operator<<=(const bigint& o);

		bigint	operator>>(int n) const;
		bigint&	operator>>=(int n);
		bigint	operator>>(const bigint& o) const;
		bigint&	operator>>=(const bigint& o);

// comparisons
		bool	operator<(const bigint& o) const;
		bool	operator<=(const bigint& o) const;
		bool	operator>(const bigint& o) const;
		bool	operator>=(const bigint& o) const;
		bool	operator==(const bigint& o) const;
		bool	operator!=(const bigint& o) const;

// output
		friend std::ostream&	operator<<(std::ostream& out, const bigint& o);
};
