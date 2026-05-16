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
		~vect2();

		friend std::ostream&	operator<<(std::ostream &oss, const vect2& other);

		vect2	operator++(int);
		vect2&		operator++();
		vect2	operator--(int);
		vect2&		operator--();

		vect2&	operator+=(const vect2& other);
		vect2&	operator-=(const vect2& other);
		vect2&	operator*=(int scalar);

		const vect2	operator*(int scalar) const;
		const vect2	operator-(const vect2& other) const;
		const vect2	operator+(const vect2& other) const;

		int& 		operator[](int index);
		const int&	operator[](int index) const;

		vect2	operator-() const;

		bool	operator==(const vect2& other) const;
		bool	operator!=(const vect2& other) const;

		friend const vect2	operator*(int scalar, const vect2& other);
};
