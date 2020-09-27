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
		pipeline.cpp pipeline.h
		physical_device.cpp physical_device.h

		resource_factory.cpp resource_factory.h
		allocator/allocator.cpp allocator/allocator.h
		renderpass/render_pass_builder.cpp renderpass/render_pass_builder.h renderpass/render_pass.cpp renderpass/render_pass.h)

add_library(PICOFRAMEWORK ${PICOFRAMEWORK_FILES})

target_include_directories(
		PICOFRAMEWORK

		PUBLIC

		./

		${glfw3_INCLUDE_PATH}
		${LIBS_PATH}/glm-0.9.9.6
		${LIBS_PATH}/stb
		${LIBS_PATH}/vma

		${VULKAN_INCLUDE_PATH}
)

target_link_libraries(
		PICOFRAMEWORK

		glfw3
		${Vulkan_LIBRARIES}
)
