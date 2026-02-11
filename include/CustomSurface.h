#ifndef CUSTOM_SURFACE_H
#define CUSTOM_SURFACE_H

#include <memory>
#include <vulkan/vulkan_raii.hpp>
class CustomSurface{
public :
    bool CreateSurface(const std::unique_ptr<vk::raii::Instance>& vkInstance, GLFWwindow* window);
    const std::unique_ptr<vk::raii::SurfaceKHR>& GetSurface() const { return mSurface; }

private :

    VkBool32 mDeviceSupported;

    std::unique_ptr<vk::raii::SurfaceKHR> mSurface;

};

#endif// CUSTOM_SURFACE_H
