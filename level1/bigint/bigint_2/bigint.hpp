#pragma once

#include <ostream>
#include <string>

class bigint
{
	private:
		std::string	_digits;

		void	normalize();

	public:
		bigint();
		bigint(const bigint& other);
		bigint(unsigned long n);
		bigint&	operator=(const bigint& other);

		bigint	operator+(const bigint& other) const;
		bigint&	operator+=(const bigint& other);

		bigint&	operator++();
		bigint	operator++(int);

		bigint	operator<<(unsigned long n) const;
		bigint&	operator<<=(unsigned long n);
		bigint	operator>>(unsigned long n) const;
		bigint&	operator>>=(unsigned long n);

		bigint	operator<<(const bigint& other) const;
		bigint&	operator<<=(const bigint& other);
		bigint	operator>>(const bigint& other) const;
		bigint&	operator>>=(const bigint& other);

		bool	operator==(const bigint& other) const;
		bool	operator!=(const bigint& other) const;
		bool	operator<(const bigint& other) const;
		bool	operator<=(const bigint& other) const;
		bool	operator>(const bigint& other) const;
		bool	operator>=(const bigint& other) const;

		friend std::ostream&	operator<<(std::ostream& os, const bigint& other);

		friend unsigned long	to_uint(const bigint& other);
};
