#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include "VulkanLogicalDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanSurface.h"

class VulkanSwapChain {
public:

    bool CreateSwapchain(GLFWwindow* window,const std::unique_ptr<VulkanSurface>& surface,
                         const std::unique_ptr<VulkanPhysicalDevice>& pDevice, const std::unique_ptr<VulkanLogicalDevice>& lDevice);

    bool RecreateSwapChain(GLFWwindow* window,const std::unique_ptr<VulkanSurface>& surface,
                           const std::unique_ptr<VulkanPhysicalDevice>& pDevice, const std::unique_ptr<VulkanLogicalDevice>& lDevice);

    void Cleanup();

    bool CreateImageViews(const std::unique_ptr<VulkanLogicalDevice>& lDevice);

    [[nodiscard]] const std::unique_ptr<vk::raii::SwapchainKHR>& GetSwapchain() const { return mSwapChain; }
    [[nodiscard]] const std::vector<vk::Image>& GetImages() const { return mSwapChainImages; }
    [[nodiscard]] const std::vector<std::unique_ptr<vk::raii::ImageView>>& GetImageViews() const { return mSwapChainImageViews;}
    [[nodiscard]] const vk::SurfaceFormatKHR& GetFormat() const { return mSwapChainSurfaceFormat; }
    [[nodiscard]] const vk::Extent2D& GetExtent() const { return mSwapChainExtent; };


private:
    void CreateSwapInfo(vk::SwapchainCreateInfoKHR& info, const std::unique_ptr<VulkanSurface>& surface);
    void SetQueueFamilies(const std::unique_ptr<VulkanPhysicalDevice>& pDevice, vk::SwapchainCreateInfoKHR& info);

    std::unique_ptr<vk::raii::SwapchainKHR> mSwapChain;

    std::vector<vk::Image> mSwapChainImages;
    std::vector<std::unique_ptr<vk::raii::ImageView>> mSwapChainImageViews;

    vk::SurfaceFormatKHR mSwapChainSurfaceFormat;
    vk::Extent2D mSwapChainExtent;

    uint32_t mSwapMinImageCount = 0;
    uint32_t mSwapImageCount = 0;

};

#endif // SWAPCHAIN_H
