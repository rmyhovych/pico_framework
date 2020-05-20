cmake_minimum_required(VERSION 3.16)

set(TEST_FILES

		main.cpp
		)

add_executable(TEST ${TEST_FILES})

target_include_directories(TEST ./)

target_link_libraries(TEST PICOFRAMEWORK)
