#include "../include/CustomSurface.h"
#include "vulkan/vulkan.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstdint>
#include <limits>

bool CustomSurface::CreateSurface(const std::unique_ptr<vk::raii::Instance>& vkInstance, GLFWwindow* window){

    VkSurfaceKHR rawSurface;

    VkResult result = glfwCreateWindowSurface(**vkInstance, window, nullptr, &rawSurface);

    if(result != VK_SUCCESS)
    {
        std::cerr << "Failed to create Surface\n";
        return false;
    }

    mSurface = std::make_unique<vk::raii::SurfaceKHR>(*vkInstance, rawSurface);

    return true;
}

bool CustomSurface::SelectSurfaceFormat(){

    bool formatFound = false;

    for(const vk::SurfaceFormatKHR& format : mAvailableFormats)
    {
        if(format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            formatFound = true;
            mSurfaceFormat = format;
            std::cout << "Surface format found\n";
            break;
        }
    }

    if(!formatFound)
    {
        std::cerr << "Couldnt find the correct surface format\n";
        return false;
    }

    return true;
}

bool CustomSurface::SelectPresentMode(){

    bool foundMode = false;

    for(const vk::PresentModeKHR& presentMode : mAvailablePresentModes)
    {
        if(presentMode == vk::PresentModeKHR::eFifo)
        {
            foundMode = true;
            mPresentMode = presentMode;
            std::cout << "Surface Present Mode found\n";
            break;
        }
    }

    if(!foundMode)
    {
        std::cerr << "Couldnt find the correct present mode\n";
        return false;
    }

    return true;
}


bool CustomSurface::SetupSurfaceCapabilities(GLFWwindow* window){

    if (mSurfaceCapabilities.minImageCount == 0) {

        std::cerr << "No extent values available\n";
        return false;
    }

    if(mSurfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        mSurfaceExtent = mSurfaceCapabilities.currentExtent;
        return true;
    }

    glfwGetFramebufferSize(window, &mSurfaceWidth, &mSurfaceHeight);

    mSurfaceExtent.setWidth(std::clamp<uint32_t>(mSurfaceWidth, mSurfaceCapabilities.minImageExtent.width, mSurfaceCapabilities.maxImageExtent.width));
    mSurfaceExtent.setHeight(std::clamp<uint32_t>(mSurfaceHeight, mSurfaceCapabilities.minImageExtent.height, mSurfaceCapabilities.maxImageExtent.height));

    std::cout << "Surface Extent set\n";

    return true;
}

bool CustomSurface::SetupSurfaceVariables(GLFWwindow* window) {

    if(!SelectSurfaceFormat()) { return false; }
    if(!SelectPresentMode()) { return false; }
    if(!SetupSurfaceCapabilities(window)) { return false; }

    return true;
}
