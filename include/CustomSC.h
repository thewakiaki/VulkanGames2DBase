#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H



#include "CustomLD.h"
#include "CustomPD.h"
#include "CustomSurface.h"
#include "vulkan/vulkan.hpp"

class CustomSC {
public:

    bool CreateSwapchain(GLFWwindow* window,const std::unique_ptr<CustomSurface>& surface,
                         const std::unique_ptr<CustomPD>& pDevice, const std::unique_ptr<CustomLD>& lDevice);

    inline const std::unique_ptr<vk::raii::SwapchainKHR>& GetSwapchain() const { return mSwapChain; }
    inline const std::vector<vk::Image>& GetImages() const { return mSwapChainImages; }
    inline const vk::SurfaceFormatKHR& GetFormat() const { return mSwapChainSurfaceFormat; }


private:
    void CreateSwapInfo(vk::SwapchainCreateInfoKHR& info, const std::unique_ptr<CustomSurface>& surface);
    void SetQueueFamilies(const std::unique_ptr<CustomPD>& pDevice, vk::SwapchainCreateInfoKHR& info);

    std::unique_ptr<vk::raii::SwapchainKHR> mSwapChain;

    std::vector<vk::Image> mSwapChainImages;

    vk::SurfaceFormatKHR mSwapChainSurfaceFormat;
    vk::Extent2D mSwapChainExtent;

    uint32_t mSwapMinImageCount;
    uint32_t mSwapImageCount;

};

#endif // SWAPCHAIN_H
