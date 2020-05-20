cmake_minimum_required(VERSION 3.16)

project(PICOFRAMEWORK)

set (VULKAN_PATH C:/VulkanSDK/1.2.131.2)
get_filename_component (LIBS_PATH libs REALPATH)

add_subdirectory(src)
add_subdirectory(test)
