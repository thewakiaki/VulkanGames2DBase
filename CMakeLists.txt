cmake_minimum_required(VERSION 3.20)
project(VulkanGames2DBase)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(SDL3 REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_executable(VulkanGame
	source/main.cpp

	source/vksetup.cpp
)

target_precompile_headers(VulkanGame PRIVATE include/pch.h)

target_compile_features(VulkanGame PRIVATE cxx_std_17)
	
target_include_directories(VulkanGame PUBLIC include PRIVATE source)

target_link_libraries(VulkanGame
	Vulkan::Vulkan
	glfw
	SDL2
	SDL2_mixer
)
