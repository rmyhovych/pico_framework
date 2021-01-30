cmake_minimum_required(VERSION 3.16)

set(PICOFRAMEWORK_FILES
		instance.h instance.cpp
		surface.h surface.cpp
		device/device.cpp device/device.h
		device/physical_device.cpp device/physical_device.h

		window/window_manager.h
		window/window_manager.cpp

		renderpass/render_pass.cpp renderpass/render_pass.h

		resources/asset_reader.cpp resources/asset_reader.h
		resources/resource_factory.cpp resources/resource_factory.h
		resources/allocator.cpp resources/allocator.h

		swapchain/swapchain.cpp swapchain/swapchain.h

		pipeline/state/state_manager.cpp pipeline/state/state_manager.h

		pipeline/state/pipeline_state.h
		pipeline/state/rasterization_state.cpp pipeline/state/rasterization_state.h
		pipeline/state/vertex_input_state.cpp pipeline/state/vertex_input_state.h
		pipeline/state/input_assembly_state.cpp pipeline/state/input_assembly_state.h
		pipeline/state/viewport_state.cpp pipeline/state/viewport_state.h
		pipeline/state/rasterization_state.cpp pipeline/state/rasterization_state.h
		pipeline/state/multisample_state.cpp pipeline/state/multisample_state.h
		pipeline/state/color_blend_state.cpp pipeline/state/color_blend_state.h
		pipeline/state/dynamic_state.cpp pipeline/state/dynamic_state.h
		pipeline/state/depth_stencil_state.cpp pipeline/state/depth_stencil_state.h

		pipeline/shader_stages.cpp pipeline/shader_stages.h
		pipeline/layout/layout.cpp pipeline/layout/layout.h
		pipeline/pipeline.cpp pipeline/pipeline.h
		renderer/renderer.cpp renderer/renderer.h renderer/frame_manager.cpp renderer/frame_manager.h primitive/object_descriptor.h primitive/object_descriptor.cpp resources/data_transfer_manager.cpp resources/data_transfer_manager.h device/device_queue.h pipeline/layout/binding/binding_layout.h pipeline/layout/binding/binding.h pipeline/layout/binding/binding_resource.h pipeline/layout/binding/buffer_binding_resource.h pipeline/layout/binding/uniform_binding.h pipeline/layout/binding/uniform_binding_layout.h)

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
