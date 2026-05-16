#include "bigint.hpp"
#include <algorithm>

bigint::bigint() : _digits("0") {}

bigint::bigint(const bigint& other) : _digits(other._digits) {}

bigint::bigint(unsigned long n)
{
	if (n == 0)
	{
		_digits = "0";
		return ;
	}
	while (n > 0)
	{
		_digits.push_back((n % 10) + '0');
		n /= 10;
	}
	std::reverse(_digits.begin(), _digits.end());
}

bigint&	bigint::operator=(const bigint& other)
{
	if (&other != this)
		_digits = other._digits;
	return (*this);
}

bigint	bigint::operator+(const bigint& other) const
{
	int carry = 0;
	int i = _digits.size() - 1;
	int j = other._digits.size() - 1;
	std::string res;

	while (i >= 0 || j >= 0 || carry)
	{
		int x = (i >= 0) ? _digits[i] - '0' : 0;
		int y = (j >= 0) ? other._digits[i] - '0' : 0;
		int sum = x + y + carry;
		carry = sum / 10;
		res.push_back((sum % 10) + '0');
		--i;
		--j;
	}
	std::reverse(res.begin(), res.end());
	bigint result;
	result._digits = res;
	result.normalize();
	return (result);
}