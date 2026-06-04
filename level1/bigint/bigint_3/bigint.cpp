#include "bigint.hpp"

#include <algorithm>
#include <climits>

// internals
bool	bigint::toUl(unsigned long& n) const
{
	unsigned int	digit;
	unsigned long	r = 0;

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

// constructors + destructor
bigint::bigint(unsigned long n)
{
	if (n == 0)
	{
		_s = "0";
		return ;
	}
	while (n)
	{
		_s += char((n % 10) + '0');
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

// addition
bigint bigint::operator+(const bigint& o) const
{
	int		i, j, carry = 0;
	bigint	r;

	r._s.clear();
	i = (int)_s.size() - 1;
	j = (int)o._s.size() - 1;
	while (i >= 0 || j >=0 || carry)
	{
		if (i >= 0) carry += _s[i--] - '0';
		if (j >= 0) carry += o._s[j--] - '0';
		r._s += char('0' + carry % 10);
		carry /= 10;
	}
	std::reverse(r._s.begin(), r._s.end());
	return (r);
}

bigint& bigint::operator++()
{
	*this += bigint(1);
	return (*this);
}

bigint	bigint::operator++(int)
{
	bigint	clone(*this);

	*this += bigint(1);
	return (clone);
}

bigint&	bigint::operator+=(const bigint& o)
{
	*this = *this + o;
	return (*this);
}

// digits shift
bigint	bigint::operator<<(int n) const
{
	bigint	clone(*this);

	clone <<= n;
	return (clone);
}

bigint&	bigint::operator<<=(int n)
{
	if (_s != "0")
		_s.append(n, '0');
	return (*this);
}

bigint	bigint::operator>>(int n) const
{
	bigint	clone(*this);

	clone >>= n;
	return (clone);
}

bigint&	bigint::operator>>=(int n)
{
	if (n >= (int)_s.size())
		_s = "0";
	else
		_s.erase(_s.size() - n);
	return (*this);
}

bigint	bigint::operator<<(const bigint& o) const
{
	unsigned long	n;
	bigint			clone(*this);

	if (o.toUl(n))
		clone << (int)n;
	return (clone);
}

bigint&	bigint::operator<<=(const bigint& o)
{
	unsigned long	n;

	if (o.toUl(n))
		*this <<= (int)n;
	return (*this);
}

bigint	bigint::operator>>(const bigint& o) const
{
	bigint	clone(*this);

	clone >>= o;
	return (clone);
}

bigint&	bigint::operator>>=(const bigint& o)
{
	unsigned long	n;

	if (o.toUl(n))
		*this >>= (int)n;
	return (*this);
}

bool	bigint::operator<(const bigint& o) const
{
	if (_s.size() != o._s.size())
		return (_s.size() < o._s.size());
	return (_s < o._s);
}

bool	bigint::operator<=(const bigint& o) const
{
	return (!(*this > o));
}

bool	bigint::operator>(const bigint& o) const
{
	return (o < *this);
}

bool	bigint::operator>=(const bigint& o) const
{
	return (o <= *this);
}

bool	bigint::operator==(const bigint& o) const
{
	return (_s == o._s);
}

bool	bigint::operator!=(const bigint& o) const
{
	return (_s != o._s);
}

std::ostream&	operator<<(std::ostream& oss, const bigint& o)
{
	oss << o._s;
	return (oss);
}
