cmake_minimum_required(VERSION 3.16)

set(PICOFRAMEWORK_FILES
	instance.h
	instance.cpp
	surface.h
	surface.cpp
	device.cpp
	device.h
	window/window_manager.h
	window/window_manager.cpp
	physical_device.cpp physical_device.h
	resources/resource_reader.cpp resources/resource_reader.h
	resources/resource_factory.cpp resources/resource_factory.h
	resources/allocator.cpp resources/allocator.h
	renderpass/render_pass.cpp renderpass/render_pass.h
	resources/alloc_objects.h
	swapchain/swapchain.cpp swapchain/swapchain.h
	pipeline/shader.cpp pipeline/shader.h
	pipeline/state/vertex_input_state.cpp pipeline/state/vertex_input_state.h
	pipeline/state/input_assembly_state.cpp pipeline/state/input_assembly_state.h
	pipeline/state/viewport_state.cpp pipeline/state/viewport_state.h
)

add_library(PICOFRAMEWORK ${PICOFRAMEWORK_FILES})

target_include_directories(
	PICOFRAMEWORK
	PUBLIC

	./

	${LIBS_PATH}/glm-0.9.9.6
	${LIBS_PATH}/stb
	${LIBS_PATH}/vma
	${VULKAN_INCLUDE_PATH}
)

target_link_libraries(
	PICOFRAMEWORK
	${Vulkan_LIBRARIES}
)
