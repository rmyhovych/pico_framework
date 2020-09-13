cmake_minimum_required(VERSION 3.16)

project(PICOFRAMEWORK)


# VULKAN
set(VULKAN_VERSION 1.2.148.1)
if (UNIX)
    set(ENV{VULKAN_SDK} /usr/src/vulkan/${VULKAN_VERSION}/x86_64)
    find_package(Vulkan REQUIRED FATAL_ERROR)
    set(VULKAN_INCLUDE_PATH $ENV{VULKAN_SDK}/include)
endif(UNIX)

get_filename_component (LIBS_PATH libs REALPATH)


add_subdirectory(src)
add_subdirectory(test)
