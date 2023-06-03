#include <catch2/catch_test_macros.hpp>

#include <mogli/sample.hpp>

TEST_CASE("Squares", "[demo]") {
	REQUIRE(mogli::myFunc(0) == 0);
	REQUIRE(mogli::myFunc(1) == 1);
	REQUIRE(mogli::myFunc(2) == 4);
	REQUIRE(mogli::myFunc(3) == 9);
}