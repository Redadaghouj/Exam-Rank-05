#include "bigint.hpp"
#include <algorithm>
#include <climits>

// ── Internals ────────────────────────────────────────────────────────────────

// void	bigint::trim()
// {
// 	size_t	p;

// 	p = _s.find_first_not_of('0');
// 	_s = (p == std::string::npos) ? "0" : _s.substr(p);
// }

bool	bigint::to_ul(unsigned long &out) const
{
	unsigned long	r = 0;
	unsigned int	digit;

	for (size_t i = 0; i < _s.size(); i++)
	{
		digit = _s[i] - '0';
		if (r > (ULONG_MAX - digit) / 10)
			return (false);
		r = r * 10 + digit;
	}
	out = r;
	return (true);
}

// ── Constructors ─────────────────────────────────────────────────────────────

bigint::bigint() : _s("0") {}

bigint::bigint(unsigned long long n) : _s("")
{
	if (!n)
	{
		_s = "0";
		return ;
	}
	while (n)
	{
		_s += char('0' + n % 10);
		n /= 10;
	}
	std::reverse(_s.begin(), _s.end());
}

bigint::bigint(const bigint &o) : _s(o._s) {}

bigint	&bigint::operator=(const bigint &o)
{
	if (this != &o)
		_s = o._s;
	return (*this);
}

// ── Addition ─────────────────────────────────────────────────────────────────

bigint	bigint::operator+(const bigint &o) const
{
	bigint	r;
	int		i, j, carry = 0;

	r._s.clear();
	i = (int)_s.size() - 1;
	j = (int)o._s.size() - 1;
	while (i >= 0 || j >= 0 || carry)
	{
		if (i >= 0) carry += _s[i--] - '0';
		if (j >= 0) carry += o._s[j--] - '0';
		r._s += char('0' + carry % 10);
		carry /= 10;
	}
	std::reverse(r._s.begin(), r._s.end());
	return (r);
}

bigint	&bigint::operator+=(const bigint &o)
{
	*this = *this + o;
	return (*this);
}

bigint	&bigint::operator++()
{
	*this += bigint(1);
	return (*this);
}

bigint	bigint::operator++(int)
{
	bigint	t(*this);

	++(*this);
	return (t);
}

// ── Digit Shifts ─────────────────────────────────────────────────────────────

bigint	bigint::operator<<(int n) const
{
	bigint	r(*this);

	r <<= n;
	return (r);
}

bigint	&bigint::operator<<=(int n)
{
	if (_s != "0")
		_s.append(n, '0');
	return (*this);
}

bigint	bigint::operator<<(const bigint &o) const
{
	bigint		r(*this);
	unsigned long	n;

	if (o.to_ul(n))
		r <<= (int)n;
	return (r);
}

bigint	&bigint::operator<<=(const bigint &o)
{
	unsigned long	n;

	if (o.to_ul(n))
		*this <<= (int)n;
	return (*this);
}

bigint	bigint::operator>>(int n) const
{
	bigint	r(*this);

	r >>= n;
	return (r);
}

bigint	&bigint::operator>>=(int n)
{
	if (n >= (int)_s.size())
		_s = "0";
	else
		_s.erase(_s.size() - n);
	return (*this);
}

bigint	bigint::operator>>(const bigint &o) const
{
	bigint	r(*this);

	r >>= o;
	return (r);
}

bigint	&bigint::operator>>=(const bigint &o)
{
	unsigned long	n;

	if (!o.to_ul(n) || n >= _s.size())
		_s = "0";
	else
		_s.erase(_s.size() - n);
	return (*this);
}

// ── Comparisons ──────────────────────────────────────────────────────────────

bool	bigint::operator<(const bigint &o)  const
{
	if (_s.size() != o._s.size())
		return (_s.size() < o._s.size());
	return (_s < o._s);
}

bool	bigint::operator==(const bigint &o) const { return (_s == o._s); }

bool	bigint::operator!=(const bigint &o) const { return (_s != o._s); }

bool	bigint::operator<=(const bigint &o) const { return (!(*this > o)); }

bool	bigint::operator>(const bigint &o)  const { return (o < *this); }

bool	bigint::operator>=(const bigint &o) const { return (!(*this < o)); }

// ── Output ───────────────────────────────────────────────────────────────────

std::ostream	&operator<<(std::ostream &os, const bigint &b)
{
	return (os << b._s);
}
