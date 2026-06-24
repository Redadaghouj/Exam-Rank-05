#include <ostream>
#include <string>

class bigint
{
	private:
		std::string	_s;

		bool	to_ul(unsigned long& n) const;

	public:
		bigint() : _s("0") {}
		bigint(unsigned long n);
		bigint(const bigint& o) : _s(o._s) {}
		bigint& operator=(const bigint& o);
		~bigint() {}

		friend std::ostream& operator<<(std::ostream& oss, const bigint& o);

		bigint	operator+(const bigint& o) const;
		bigint&	operator++();
		bigint	operator++(int);
		bigint& operator+=(const bigint& o);

		bigint	operator<<(int n);
		bigint	operator>>(int n);
		bigint&	operator<<=(int n);
		bigint&	operator>>=(int n);

		bigint	operator<<(const bigint& o);
		bigint	operator>>(const bigint& o);
		bigint&	operator<<=(const bigint& o);
		bigint&	operator>>=(const bigint& o);

		bool	operator<(const bigint& o) const;
		bool	operator<=(const bigint& o) const;
		bool	operator>(const bigint& o) const;
		bool	operator>=(const bigint& o) const;
		bool	operator==(const bigint& o) const;
		bool	operator!=(const bigint& o) const;
};
