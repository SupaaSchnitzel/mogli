cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/include/mogli/mogli.hpp" MOGLI_VERSION_MACRO REGEX "^\tconstexpr const auto version = \"(([0-9]+\\.)*[0-9]+)\";$")
string(REGEX REPLACE "^\tconstexpr const auto version = \"(([0-9]+\\.)*[0-9]+)\"\\;$" "\\1" MOGLI_VERSION ${MOGLI_VERSION_MACRO})

project(mogli VERSION ${MOGLI_VERSION} LANGUAGES CXX)

option(BUILD_TESTS "Build tests" ON)


add_subdirectory(src)

##########################################################################################
# Testing
##########################################################################################
# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()
