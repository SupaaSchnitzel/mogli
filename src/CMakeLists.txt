
# Library
add_library(mogli STATIC
	mogli/sample.cpp
)
target_compile_features(mogli PUBLIC cxx_std_23)
target_include_directories(mogli PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../include)

# Executable
add_executable(mogliexe main.cpp)
target_compile_features(mogliexe PUBLIC cxx_std_23)
target_link_libraries(mogliexe mogli)