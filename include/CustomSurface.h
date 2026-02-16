#ifndef CUSTOM_SURFACE_H
#define CUSTOM_SURFACE_H

#include "vulkan/vulkan.hpp"
#include <memory>
#include <vector>
#include <vulkan/vulkan_raii.hpp>
class CustomSurface{
    public :
        bool CreateSurface(const std::unique_ptr<vk::raii::Instance>& vkInstance, GLFWwindow* window);

        inline void SetCapabilities(const std::unique_ptr<vk::raii::PhysicalDevice>& device) { mSurfaceCapabilities = device->getSurfaceCapabilitiesKHR(*mSurface); }
        inline void SetFormats(const std::unique_ptr<vk::raii::PhysicalDevice>& device) { mAvailableFormats = device->getSurfaceFormatsKHR(*mSurface); };
        inline void SetPresentModes(const std::unique_ptr<vk::raii::PhysicalDevice>& device) { mAvailablePresentModes = device->getSurfacePresentModesKHR(*mSurface); }

        bool SetupSurfaceVariables(GLFWwindow* window);

        inline const vk::SurfaceCapabilitiesKHR& GetCapabilities() const { return mSurfaceCapabilities; }
        inline const std::vector<vk::SurfaceFormatKHR>& GetAvailableFormats() const { return mAvailableFormats; }
        inline const std::vector<vk::PresentModeKHR>& GetPresentModes() const { return mAvailablePresentModes; }
        inline const std::unique_ptr<vk::raii::SurfaceKHR>& GetSurface() const { return mSurface; }

        inline const vk::SurfaceFormatKHR GetChosenFormat() const { return mSurfaceFormat; }
        inline const vk::Extent2D GetChosenExtent() const { return mSurfaceExtent; }
        inline const vk::PresentModeKHR GetChosenPresentMode() const { return mPresentMode; }

    private :
        bool SelectSurfaceFormat();
        bool SelectPresentMode();
        bool SetupSurfaceCapabilities(GLFWwindow* window);

        VkBool32 mDeviceSupported;

        std::unique_ptr<vk::raii::SurfaceKHR> mSurface;

        vk::SurfaceCapabilitiesKHR mSurfaceCapabilities;
        std::vector<vk::SurfaceFormatKHR> mAvailableFormats;
        std::vector<vk::PresentModeKHR> mAvailablePresentModes;

        vk::SurfaceFormatKHR mSurfaceFormat;
        vk::PresentModeKHR mPresentMode;
        vk::Extent2D mSurfaceExtent;

        int mSurfaceWidth;
        int mSurfaceHeight;
};

#endif// CUSTOM_SURFACE_H
