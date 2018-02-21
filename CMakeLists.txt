cmake_minimum_required(VERSION 3.1.3)
project(lip CXX)

include(CTest)

if(NOT MSVC)
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
	    ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
elseif(VCPKG_TARGET_TRIPLET MATCHES "x..-windows-static")
	foreach(flagvar CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
	    CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
		string(REGEX REPLACE "/MD" "/MT" ${flagvar} "${${flagvar}}")
		string(REGEX REPLACE "/MDd" "/MTd" ${flagvar} "${${flagvar}}")
	endforeach()
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY tests)
set(CMAKE_CXX_STANDARD 14)

if(MINGW)
	add_definitions(-DMINGW_HAS_SECURE_API)
endif()

if(NOT MSVC)
	add_definitions(-Wall -Wconversion -Wsign-conversion -Wsign-compare)
	set(CMAKE_CXX_EXTENSIONS OFF)
else()
	add_definitions(/std:c++latest)
endif()

include_directories(3rdparty/include)
include_directories(include)

file(GLOB lip_srcs src/*.cc)
file(GLOB tests_srcs tests/*.cc)

add_library(lip ${lip_srcs})

if(BUILD_TESTING)
	add_executable(run ${tests_srcs})
	target_link_libraries(run lip)
	add_test(testall tests/run)
endif()

file(GLOB demo_srcs demo/*.cc)

foreach(demo_src ${demo_srcs})
	get_filename_component(demo ${demo_src} NAME_WE)
	add_executable(${demo} ${demo_src})
	target_link_libraries(${demo} lip)
	set_target_properties(${demo} PROPERTIES RUNTIME_OUTPUT_DIRECTORY demo)
endforeach()