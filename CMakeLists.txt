cmake_minimum_required(VERSION 3.16)
set(CMAKE_BUILD_TYPE Debug)

project(PICOFRAMEWORK)

# GLFW 3
if (UNIX)
	find_package(glfw3 3.3 REQUIRED FATAL_ERROR)
	set(glfw3_INCLUDE_PATH /usr/local/include/GLFW/glfw3)

	set(glfw3_LIBS dl GL GLU glfw3 X11 Xxf86vm Xrandr pthread Xi)
endif ()


# VULKAN
set(VULKAN_VERSION 1.2.148.1)
if (UNIX)
	set(ENV{VULKAN_SDK} /usr/src/vulkan/${VULKAN_VERSION}/x86_64)
	find_package(Vulkan REQUIRED FATAL_ERROR)
	set(VULKAN_INCLUDE_PATH $ENV{VULKAN_SDK}/include)
endif (UNIX)

get_filename_component(LIBS_PATH libs REALPATH)

add_subdirectory(src)
add_subdirectory(test)

