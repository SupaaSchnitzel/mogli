include(FetchContent)

# Library
add_library(mogli STATIC
	mogli/libmgr/manager.cpp
	mogli/libmgr/database/postgresql.cpp
	mogli/libmgr/filesystem/gameentry.cpp
	mogli/libmgr/filesystem/scanner.cpp
	mogli/libmgr/provider/gogprovider.cpp
	mogli/libmgr/provider/gogdbprovider.cpp
	mogli/libmgr/provider/steamprovider.cpp
	mogli/libmgr/provider/localprovider.cpp
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

# spdlog for logging
FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG v1.11.0)
FetchContent_MakeAvailable(spdlog)
target_link_libraries(mogli PUBLIC spdlog::spdlog)
target_link_libraries(mogliexe spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

# CLI11 for configurations via .ini files and CLI
FetchContent_Declare(cli11 GIT_REPOSITORY https://github.com/CLIUtils/CLI11 GIT_TAG v2.3.2)
FetchContent_MakeAvailable(cli11)
target_link_libraries(mogliexe CLI11::CLI11)

# rapidyaml for adding meta information to game folders
FetchContent_Declare(ryml
    GIT_REPOSITORY https://github.com/biojppm/rapidyaml.git
    GIT_TAG v0.5.0
    GIT_SHALLOW FALSE  # ensure submodules are checked out
)
FetchContent_MakeAvailable(ryml)
target_link_libraries(mogli PUBLIC ryml::ryml)
target_link_libraries(mogliexe ryml::ryml)

# SOCI for abstracting database backends
set(SOCI_TESTS OFF)
set(SOCI_EMPTY OFF)
set(SOCI_CXX11 ON)
set(WITH_DB2 OFF)
set(WITH_FIREBIRD OFF)
set(WITH_MYSQL OFF)
set(WITH_ORACLE OFF)
set(SOCI_POSTGRESQL ON) # requires libpq-dev to be installed
set(WITH_SQLITE3 OFF)
# FetchContent_Declare(soci GIT_REPOSITORY https://github.com/SOCI/soci.git GIT_TAG v4.0.3)
FetchContent_Declare(soci GIT_REPOSITORY https://github.com/SOCI/soci.git GIT_TAG 924d990f8f4b253e9f7897c92dccfd4c814f569e)
FetchContent_MakeAvailable(soci)
set_property(TARGET soci_core PROPERTY CXX_STANDARD 17)
target_link_libraries(mogli PRIVATE soci_core soci_postgresql)
target_include_directories(mogli PRIVATE ${soci_BINARY_DIR}/include ${POSTGRESQL_INCLUDE_DIR})