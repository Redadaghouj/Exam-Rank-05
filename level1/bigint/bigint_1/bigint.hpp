#pragma once
#include <iostream>
#include <string>

class bigint
{
	private:
// ── Internals ────────────────────────────────────────────────────────────────
		std::string	_s;

		// void			trim();
		bool			to_ul(unsigned long &out) const;

	public:
// ── Constructors ─────────────────────────────────────────────────────────────
		bigint();
		bigint(unsigned long long n);
		bigint(const bigint &o);
		bigint	&operator=(const bigint &o);

// ── Addition ─────────────────────────────────────────────────────────────────
		bigint	operator+(const bigint &o)  const;
		bigint	&operator+=(const bigint &o);
		bigint	&operator++();
		bigint	operator++(int);

// ── Digit Shifts ─────────────────────────────────────────────────────────────
		bigint	operator<<(int n)           const;
		bigint	&operator<<=(int n);
		bigint	operator<<(const bigint &o) const;
		bigint	&operator<<=(const bigint &o);

		bigint	operator>>(int n)           const;
		bigint	&operator>>=(int n);
		bigint	operator>>(const bigint &o) const;
		bigint	&operator>>=(const bigint &o);

// ── Comparisons ──────────────────────────────────────────────────────────────
		bool	operator==(const bigint &o) const;
		bool	operator!=(const bigint &o) const;
		bool	operator<(const bigint &o)  const;
		bool	operator<=(const bigint &o) const;
		bool	operator>(const bigint &o)  const;
		bool	operator>=(const bigint &o) const;

// ── Output ───────────────────────────────────────────────────────────────────
		friend std::ostream	&operator<<(std::ostream &os, const bigint &b);
};