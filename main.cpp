#include <iostream>
int main()
{
	int x = 5;
	int b = x+=3;
	b += 2;
	std::cout << x << std::endl;
	std::cout << b << std::endl;
}
