#include "../include/CustomSurface.h"

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
