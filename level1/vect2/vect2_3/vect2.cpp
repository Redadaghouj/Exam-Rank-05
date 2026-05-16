#include "vect2.hpp"

vect2& vect2::operator=(const vect2& other)
{
	if (this != &other)
	{
		_x = other._x;
		_y = other._y;
	}
	return (*this);
}

std::ostream&	operator<<(std::ostream& os, const vect2& other)
{
	os << "{" << other._x << ", " << other._y << "}";
	return (os);
}

const int&	vect2::operator[](int index) const
{
	return (index == 0 ? _x : _y);
}

int&	vect2::operator[](int index)
{
	return (index == 0 ? _x : _y);
}

vect2	vect2::operator++(int)
{
	return (vect2(_x++, _y++));
}

vect2&	vect2::operator++()
{
	_x++;
	_y++;
	return (*this);
}


vect2	vect2::operator--(int)
{
	return (vect2(_x--, _y--));
}

vect2&	vect2::operator--()
{
	_x--;
	_y--;
	return (*this);
}

vect2& vect2::operator+=(const vect2& other)
{
	_x += other._x;
	_y += other._y;
	return (*this);
}

vect2& vect2::operator-=(const vect2& other)
{
	_x -= other._x;
	_y -= other._y;
	return (*this);
}

vect2& vect2::operator*=(int scalar)
{
	_x *= scalar;
	_y *= scalar;
	return (*this);
}

vect2 vect2::operator+(const vect2& other) const
{
	return (vect2(_x + other._x, _y + other._y));
}

vect2 vect2::operator-(const vect2& other) const
{
	return (vect2(_x - other._x, _y - other._y));
}

vect2 vect2::operator*(int scalar) const
{
	return (vect2(_x * scalar, _y * scalar));
}

vect2	vect2::operator-() const
{
	return (vect2(_x * -1, _y * -1));
}

bool	vect2::operator==(const vect2& other) const
{
	return (_x == other._x && _y == other._y);
}

bool	vect2::operator!=(const vect2& other) const
{
	return (_x != other._x || _y != other._y);
}

vect2	operator*(int scalar, const vect2& other)
{
	return (vect2(scalar * other._x, scalar * other._y));
}