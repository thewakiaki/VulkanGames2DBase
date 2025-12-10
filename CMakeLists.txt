cmake_minimum_required(VERSION 3.20)
project(VulkanGames2DBase)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_BUILD_TYPE Debug CACHE STRING ("Choose Debug or Release build" FORCE))

add_executable(VulkanGame
	source/main.cpp
	)
	
target_include_directories(VulkanGame PUBLIC include PRIVATE source)

target_link_libraries(VulkanGame
	Vulkan::Vulkan
	glfw
	SDL2
	SDL2_mixer
)