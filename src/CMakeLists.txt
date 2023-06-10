include(FetchContent)

# Library
add_library(mogli STATIC
	mogli/libmgr/manager.cpp
	mogli/rest/controller/controller.cpp
	mogli/rest/endpoint.cpp
	mogli/logging.cpp
	mogli/sample.cpp
)
target_compile_features(mogli PUBLIC cxx_std_23)
target_include_directories(mogli PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../include)

# Executable
add_executable(mogliexe main.cpp)
target_compile_features(mogliexe PUBLIC cxx_std_23)
target_link_libraries(mogliexe mogli)



##########################################################################################
# Libraries
##########################################################################################
# Oatpp for REST API
set(OATPP_BUILD_TESTS OFF) #We do not want oatpp to run their tests in our project
option(OATPP_BUILD_TESTS OFF)
FetchContent_Declare(
	oatpp_lib
	GIT_REPOSITORY https://github.com/oatpp/oatpp.git
	GIT_TAG 1.3.0
)
FetchContent_MakeAvailable(oatpp_lib)
FetchContent_GetProperties(oatpp_lib)
target_include_directories(mogli PRIVATE ${oatpp_lib_SOURCE_DIR}/src/)
target_include_directories(mogliexe PRIVATE ${oatpp_lib_SOURCE_DIR}/src/)
target_link_libraries(mogli PRIVATE oatpp)
#Config for OATPP Modules
set(OATPP_MODULES_LOCATION "CUSTOM")
set(OATPP_DIR_SRC ${oatpp_lib_SOURCE_DIR}/src/)
set(OATPP_DIR_LIB ${oatpp_lib_BINARY_DIR}/src/)

if (false)
	# Oatpp-postgres for REST API
	FetchContent_Declare(
		oatpp_postgres
		GIT_REPOSITORY https://github.com/oatpp/oatpp-postgresql.git
		GIT_TAG 1.3.0
	)
	FetchContent_MakeAvailable(oatpp_postgres)
	FetchContent_GetProperties(oatpp_postgres)
	target_include_directories(mogli PRIVATE ${oatpp_postgres_SOURCE_DIR}/src/)
	target_include_directories(mogliexe PRIVATE ${oatpp_postgres_SOURCE_DIR}/src/)
	target_link_libraries(mogli PRIVATE oatpp-postgresql)
endif()

# spdlog for logging
FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG v1.11.0)
FetchContent_MakeAvailable(spdlog)
target_link_libraries(mogli PRIVATE spdlog::spdlog)
# target_include_directories(mogli PUBLIC ${spdlog_SOURCE_DIR}/include)
target_link_libraries(mogliexe spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
# target_include_directories(mogliexe PUBLIC ${spdlog_SOURCE_DIR}/include)