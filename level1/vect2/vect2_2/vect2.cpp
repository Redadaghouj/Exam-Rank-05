#include "vect2.hpp"
#include <ostream>

vect2::vect2() : _x(0), _y(0) {}

vect2::vect2(int x, int y) : _x(x), _y(y) {}

vect2::vect2(const vect2& other) : _x(other._x),  _y(other._y) {}

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

vect2& vect2::operator++()
{
	++_x;
	++_y;
	return (*this);
}

vect2 vect2::operator++(int)
{
	vect2 v(_x, _y);
	++_x;
	++_y;
	return (v);
}

vect2& vect2::operator--()
{
	--_x;
	--_y;
	return (*this);
}

vect2 vect2::operator--(int)
{
	vect2 v(_x, _y);
	--_x;
	--_y;
	return (v);
}

vect2 vect2::operator+(const vect2& other) const
{
	vect2 v(_x + other._x, _y + other._y);
	return (v);
}

vect2& vect2::operator+=(const vect2& other)
{
	_x += other._x;
	_y += other._y;
	return (*this);
}

vect2 vect2::operator-(const vect2& other) const
{
	vect2 v(_x - other._x, _y - other._y);
	return (v);
}

vect2& vect2::operator-=(const vect2& other)
{
	_x -= other._x;
	_y -= other._y;
	return (*this);
}

vect2 vect2::operator*(int scalar) const
{
	vect2 v(_x * scalar, _y * scalar);
	return (v);
}

vect2& vect2::operator*=(int scalar)
{
	_x *= scalar;;
	_y *= scalar;;
	return (*this);
}

vect2	vect2::operator-() const
{
	vect2 v(_x * -1, _y * -1);
	return (v);
}

const int& vect2::operator[](int index) const
{
	return (index == 0 ? _x : _y);
}

int& vect2::operator[](int index)
{
	return (index == 0 ? _x : _y);
}

bool	vect2::operator==(const vect2& other) const
{
	return (_x == other._x && _y == other._y);
}

bool	vect2::operator!=(const vect2& other) const
{
	return (_x != other._x && _y != other._y);
}

vect2	operator*(int scalar, const vect2& other)
{
	vect2 v(other._x * scalar, other._y * scalar);
	return (v);
}
