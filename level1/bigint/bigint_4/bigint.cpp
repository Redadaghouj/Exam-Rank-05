#include "bigint.hpp"
#include <algorithm>
#include <climits>
#include <cstddef>
#include <ostream>

bigint::bigint(unsigned long n)
{
	if (n == 0)
	{
		_s = "0";
		return ;
	}
	while (n)
	{
		_s += char('0' + (n % 10));
		n /= 10;
	}
	std::reverse(_s.begin(), _s.end());
}

bigint&	bigint::operator=(const bigint& o)
{
	if (this != &o)
		_s = o._s;
	return (*this);
}

bool	bigint::to_ul(unsigned long& n) const
{
	unsigned long digit;
	unsigned long r = 0;

	for (size_t i = 0; i < _s.size(); i++)
	{
		digit = _s[i] - '0';
		if (r > (ULONG_MAX - digit) / 10)
			return (false);
		r = r * 10 + digit;
	}
	n = r;
	return (true);
}

std::ostream&	operator<<(std::ostream& oss, const bigint& o)
{
	oss << o._s;
	return (oss);
}

bigint	bigint::operator+(const bigint& o) const
{
	int carry = 0, i, j;

	i = (int)_s.size() - 1;
	j = (int)o._s.size() - 1;

	while (i >= 0 || j >= 0 || carry)
	{
		
	}
}
