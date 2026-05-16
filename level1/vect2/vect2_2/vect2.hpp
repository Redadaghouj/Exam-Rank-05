#pragma once

#include <ostream>

class vect2
{
	private:
		int	_x;
		int	_y;

	public:
		vect2();
		vect2(int x, int y);
		vect2(const vect2& other);
		vect2& operator=(const vect2& other);

		friend std::ostream&	operator<<(std::ostream& os, const vect2& other);

		vect2&	operator++();
		vect2	operator++(int);
		vect2&	operator--();
		vect2	operator--(int);

		vect2	operator+(const vect2& other) const;
		vect2&	operator+=(const vect2& other);
		vect2	operator-(const vect2& other) const;
		vect2&	operator-=(const vect2& other);
		vect2	operator*(int scalar) const;
		vect2&	operator*=(int scalar);

		vect2	operator-() const;

		const int&	operator[](int index) const;
		int&		operator[](int index);

		bool	operator==(const vect2& other) const;
		bool	operator!=(const vect2& other) const;

		friend vect2	operator*(int scalar, const vect2& other);
};
