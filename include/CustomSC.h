#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include "CustomLD.h"
#include "CustomPD.h"
#include "CustomSurface.h"

class CustomSC {
public:

    bool CreateSwapchain(GLFWwindow* window,const std::unique_ptr<CustomSurface>& surface,
                         const std::unique_ptr<CustomPD>& pDevice, const std::unique_ptr<CustomLD>& lDevice);

    bool RecreateSwapChain(GLFWwindow* window,const std::unique_ptr<CustomSurface>& surface,
                           const std::unique_ptr<CustomPD>& pDevice, const std::unique_ptr<CustomLD>& lDevice);

    void CleanupSwapchain();

    bool CreateImageViews(const std::unique_ptr<CustomLD>& lDevice);

    inline const std::unique_ptr<vk::raii::SwapchainKHR>& GetSwapchain() const { return mSwapChain; }
    inline const std::vector<vk::Image>& GetImages() const { return mSwapChainImages; }
    inline const std::vector<std::unique_ptr<vk::raii::ImageView>>& GetImageViews() const { return mSwapChainImageViews;}
    inline const vk::SurfaceFormatKHR& GetFormat() const { return mSwapChainSurfaceFormat; }
    inline const vk::Extent2D& GetExtent() const { return mSwapChainExtent; };


private:
    void CreateSwapInfo(vk::SwapchainCreateInfoKHR& info, const std::unique_ptr<CustomSurface>& surface);
    void SetQueueFamilies(const std::unique_ptr<CustomPD>& pDevice, vk::SwapchainCreateInfoKHR& info);

    std::unique_ptr<vk::raii::SwapchainKHR> mSwapChain;

    std::vector<vk::Image> mSwapChainImages;
    std::vector<std::unique_ptr<vk::raii::ImageView>> mSwapChainImageViews;

    vk::SurfaceFormatKHR mSwapChainSurfaceFormat;
    vk::Extent2D mSwapChainExtent;

    uint32_t mSwapMinImageCount;
    uint32_t mSwapImageCount;

    size_t mImageIndex;

};

#endif // SWAPCHAIN_H
