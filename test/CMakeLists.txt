cmake_minimum_required(VERSION 3.16)

set(TEST_FILES

		window/glfw_window_manager.h
		window/glfw_window_manager.cpp
		instance_test.cpp)

FIND_PACKAGE(Threads REQUIRED)

add_executable(TEST ${TEST_FILES})

target_include_directories(TEST PUBLIC ./ ${glfw3_INCLUDE_PATH})
target_link_libraries(TEST
		PICOFRAMEWORK

		${glfw3_LIBS}
		)
