#include "vect2.hpp"
#include <ostream>

vect2::vect2() {}

vect2::vect2(int x, int y) : _x(x), _y(y) {}

vect2::vect2(const vect2& other) : _x(other._x), _y(other._y) {}

vect2& vect2::operator=(const vect2& other)
{
	if (this != &other)
	{
		_x = other._x;
		_y = other._y;
	}
	return (*this);
}

std::ostream&	operator<<(std::ostream &oss, const vect2& other)
{
	oss << "{" << other._x << ", " << other._y << "}";
	return (oss);
}

vect2 vect2::operator++(int)
{
	vect2 v(*this);

	_x++;
	_y++;
	return (v);
}

vect2& vect2::operator++()
{
	_x++;
	_y++;
	return (*this);
}

vect2 vect2::operator--(int)
{
	vect2 v(*this);

	_x--;
	_y--;
	return (v);
}

vect2& vect2::operator--()
{
	_x--;
	_y--;
	return (*this);
}

vect2&	vect2::operator+=(const vect2& other)
{
	_x += other._x;
	_y += other._y;
	return (*this);
}

vect2&	vect2::operator-=(const vect2& other)
{
	_x -= other._x;
	_y -= other._y;
	return (*this);
}

vect2&	vect2::operator*=(int scalar)
{
	_x *= scalar;
	_y *= scalar;
	return (*this);
}

const vect2	vect2::operator*(int scalar) const
{
	return (vect2(_x * scalar, _y * scalar));
}

const vect2	vect2::operator-(const vect2& other) const
{
	return (vect2(_x - other._x, _y - other._y));
}

const vect2	vect2::operator+(const vect2& other) const
{
	return (vect2(_x + other._x, _y + other._y));
}

int&	vect2::operator[](int index)
{
	return (index == 0 ? _x : _y);
}

const int&	vect2::operator[](int index) const
{
	return (index == 0 ? _x : _y);
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

const vect2 operator*(int scalar, const vect2& other)
{
	return (vect2(other._x * scalar, other._y * scalar));
}

vect2::~vect2() {}
