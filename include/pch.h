#ifndef PCH_H
#define PCH_H

#define GLFW_INCLUDE_VULKAN
#define FAMILY_NOT_FOUND -1

#define VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC 1

#include <vulkan/vulkan_raii.hpp>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <glm/glm.hpp>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstdint>
#include <memory>
#include <map>
#include <vector>
#include <array>
#include <ios>
#include <fstream>
#include <functional>
#include <limits>

#endif // PCH_H
