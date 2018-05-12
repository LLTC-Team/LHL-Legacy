#define CATCH_CONFIG_MAIN

#include <iostream>
#include "catch.hpp"

unsigned int Factorial(unsigned int number) {
	return number <= 1 ? number : Factorial(number - 1) * number;
}

TEST_CASE("Factorials are computed", "[factorial]") {
	REQUIRE(Factorial(1) == 1);
	REQUIRE(Factorial(2) == 2);
	REQUIRE(Factorial(3) == 6);
	REQUIRE(Factorial(10) == 3628800);
}

TEST_CASE("Hello Catch2", "[hello]"){
	std::cout << "hello catch2" << std::endl;
	std::cout << "LML test" << std::endl;
	REQUIRE(true);
}