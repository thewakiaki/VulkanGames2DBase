#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H



#include "CustomLD.h"
#include "CustomPD.h"
#include "CustomSurface.h"
#include "vulkan/vulkan.hpp"
#include <cstdint>
#include <memory>
#include <vulkan/vulkan_core.h>
class Swapchain {
public:

    bool CreateSwapchain(GLFWwindow* window,const std::unique_ptr<CustomSurface>& surface,
                         const std::unique_ptr<CustomPD>& pDevice, const std::unique_ptr<CustomLD>& lDevice);

    void CreateSwapInfo(vk::SwapchainCreateInfoKHR& info, const std::unique_ptr<CustomSurface>& surface);
    void SetQueueFamilies(const std::unique_ptr<CustomPD>& pDevice, vk::SwapchainCreateInfoKHR& info);

private:

    std::unique_ptr<vk::raii::SwapchainKHR> mSwapChain;

    std::vector<vk::Image> mSwapChainImages;

    vk::SurfaceFormatKHR mSwapChainSurfaceFormat;
    vk::Extent2D mSwapChainExtent;

    uint32_t mSwapMinImageCount;
    uint32_t mSwapImageCount;

};

#endif // SWAPCHAIN_H
