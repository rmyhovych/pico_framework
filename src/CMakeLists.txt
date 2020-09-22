cmake_minimum_required(VERSION 3.16)

set(PICOFRAMEWORK_FILES

		instance.h
		instance.cpp

		surface.h
		surface.cpp
		device.cpp
		device.h
		window/window_manager.h
		window/glfw_window_manager.cpp
		window/glfw_window_manager.h
		window/window_manager.cpp
		renderer.cpp renderer.h
		swapchain.cpp swapchain.h
		pipeline.cpp pipeline.h
		render_pass_builder.cpp render_pass_builder.h
		physical_device.cpp physical_device.h

		resource_factory.cpp resource_factory.h
		allocator/allocator.cpp allocator/allocator.h
		)

add_library(PICOFRAMEWORK ${PICOFRAMEWORK_FILES})

message("MESSAGE : ${VULKAN_PATH}")

target_include_directories(
		PICOFRAMEWORK

		PUBLIC

		./

		${LIBS_PATH}/glfw-3.3.2/include
		${LIBS_PATH}/glm-0.9.9.6
		${LIBS_PATH}/stb
		${LIBS_PATH}/vma

		${VULKAN_INCLUDE_PATH}
)

target_link_libraries(
		PICOFRAMEWORK

		${LIBS_PATH}/glfw-3.3.2/lib/glfw3.lib
		Vulkan
)
