cmake_minimum_required(VERSION 3.20)
project(VulkanGames2DBase)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, defaulting to Debug")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the build type" FORCE)
endif()

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(SDL3 REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_executable(VulkanGame
	source/main.cpp

        source/GameApp.cpp
		source/GameWindow.cpp
		include/GameWindow.h
		source/VkSetup.cpp
		include/VkSetup.h
		source/CustomPD.cpp
		include/CustomPD.h
		include/CustomVkStructs.h
		include/CustomLD.h
		source/CustomLD.cpp
		include/CustomSurface.h
		source/CustomSurface.cpp
		include/CustomSC.h
		source/CustomSC.cpp
		include/CustomIV.h
		source/CustomIV.cpp
		include/CustomSM.h
		source/CustomSM.cpp
		include/FileUtility.h
		source/FileUtility.cpp
		include/GraphicsPipeline.h
		source/GraphicsPipeline.cpp
		include/CommandBuffer.h
		source/CommandBuffer.cpp
		include/Renderer.h
		source/Renderer.cpp
)

target_precompile_headers(VulkanGame PRIVATE include/pch.h)

target_compile_definitions(VulkanGame PRIVATE
    VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
    VK_NO_PROTOTYPES=1
)

target_compile_features(VulkanGame PRIVATE cxx_std_17)

target_include_directories(VulkanGame PUBLIC include PRIVATE source)

target_link_libraries(VulkanGame
	Vulkan::Vulkan
	glfw
	SDL2
	SDL2_mixer
)
