
#include <ostream>

class vect2
{
	private:
		int	_x;
		int	_y;

	public:
		vect2() : _x(0), _y(0) {}
		vect2(int x, int y) : _x(x), _y(y) {}
		vect2(const vect2& o) : _x(o._x), _y(o._y) {}
		vect2&	operator=(const vect2& o);
		~vect2() {}

		friend	std::ostream&	operator<<(std::ostream& oss, const vect2& o);

		vect2&	operator++();
		vect2	operator++(int);
		vect2&	operator--();
		vect2	operator--(int);

		vect2&	operator+=(const vect2& o);
		vect2&	operator-=(const vect2& o);
		vect2&	operator*=(int scalar);

		const vect2	operator+(const vect2& o) const;
		const vect2	operator-(const vect2& o) const;
		const vect2	operator*(int scalar) const;
		
		vect2&		operator-();

		int&		operator[](int index);
		const int&	operator[](int index) const;

		bool	operator==(const vect2& o) const;
		bool	operator!=(const vect2& o) const;

		friend const vect2	operator*(int scalar, const vect2& o);
};
