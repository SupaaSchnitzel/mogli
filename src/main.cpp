#include <iostream>

#include <mogli/sample.hpp>

int main(int argc, char* argv[]) {
	std::cout << 0 << " -> " << mogli::myFunc(0) << std::endl;
	std::cout << 1 << " -> " << mogli::myFunc(1) << std::endl;
	std::cout << 2 << " -> " << mogli::myFunc(2) << std::endl;
	std::cout << 3 << " -> " << mogli::myFunc(3) << std::endl;
	return 0;
}
