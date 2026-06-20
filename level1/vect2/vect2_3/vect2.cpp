#include "vect2.hpp"
#include <ostream>

vect2& vect2::operator=(const vect2& o)
{
	if (&o != this)
	{
		_x = o._x;
		_y = o._y;
	}
	return (*this);
}

std::ostream&	operator<<(std::ostream& oss, const vect2& o)
{
	oss << "{" << o._x << ", " << o._y << "}";
	return (oss);
}

vect2& vect2::operator++()
{
	_x++;
	_y++;
	return (*this);
}

vect2 vect2::operator++(int)
{
	vect2 clone(*this);

	++(*this);
	return (clone);
}

vect2& vect2::operator--()
{
	_x--;
	_y--;
	return (*this);
}

vect2 vect2::operator--(int)
{
	vect2 clone(*this);

	--(*this);
	return (clone);
}

vect2& vect2::operator+=(const vect2& o)
{
	_x += o._x;
	_y += o._y;
	return (*this);
}

vect2& vect2::operator-=(const vect2& o)
{
	_x -= o._x;
	_y -= o._y;
	return (*this);
}

vect2& vect2::operator*=(int scalar)
{
	_x *= scalar;
	_y *= scalar;
	return (*this);
}

const vect2	vect2::operator+(const vect2& o) const
{
	vect2 clone(*this);

	clone += o;
	return (clone);
}

const vect2	vect2::operator-(const vect2& o) const
{
	vect2 clone(*this);

	clone -= o;
	return (clone);
}

const vect2	vect2::operator*(int scalar) const
{
	vect2 clone(*this);

	clone *= scalar;
	return (clone);
}

vect2& vect2::operator-()
{
	*this *= -1;
	return (*this);
}

int&	vect2::operator[](int index)
{
	return (index == 0 ? _x : _y);
}

const int&	vect2::operator[](int index) const
{
	return (index == 0 ? _x : _y);
}

bool	vect2::operator==(const vect2& o) const
{
	return (_x == o._x && _y == o._y);
}

bool	vect2::operator!=(const vect2& o) const
{
	return (!(*this == o));
}

const vect2	operator*(int scalar, const vect2& o)
{
	return (vect2(o._x * scalar, o._y * scalar));
}
