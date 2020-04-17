#include <iostream>
#include <tuple>

int main() {
	std::cout << std::apply([](auto&& ... args) { return (args + ...);}, std::make_tuple(1, 2.f, 3.0)) << std::endl;
	
	std::apply([](auto&& ... args) { ((std::cout << args << std::endl), ...); }, std::make_tuple(1, 2.f, 3.0));
	return 0;
}

