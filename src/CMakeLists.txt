cmake_minimum_required(VERSION 3.16)

set(PICOFRAMEWORK_FILES

		instance.h
		instance.cpp

		surface.h
		surface.cpp
		device.cpp device.h window/i_window_manager.h window/glfw_window_manager.cpp window/glfw_window_manager.h)

add_library(PICOFRAMEWORK ${PICOFRAMEWORK_FILES})

target_include_directories(
		PICOFRAMEWORK

		PUBLIC

		./

		${LIBS_PATH}/glfw-3.3.2/include
		${LIBS_PATH}/glm-0.9.9.6
		${LIBS_PATH}/stb

		${VULKAN_PATH}/Include
)

target_link_libraries(
		PICOFRAMEWORK

		${LIBS_PATH}/glfw-3.3.2/lib/glfw3.lib
		${VULKAN_PATH}/Lib/vulkan-1.lib
)
