#include "../include/VulkanSwapChain.h"
#include "CustomVkStructs.h"


bool VulkanSwapChain::CreateSwapchain(GLFWwindow* window,const std::unique_ptr<VulkanSurface>& surface,const std::unique_ptr<VulkanPhysicalDevice>& pDevice, const std::unique_ptr<VulkanLogicalDevice>& lDevice){

    mSwapChainSurfaceFormat = surface->GetChosenFormat();
    mSwapChainExtent = surface->GetChosenExtent();

    mSwapMinImageCount = std::max(3u, surface->GetCapabilities().minImageCount);
    mSwapMinImageCount = ( surface->GetCapabilities().maxImageCount > 0 && mSwapMinImageCount > surface->GetCapabilities().maxImageCount) ? surface->GetCapabilities().maxImageCount : mSwapMinImageCount;

    mSwapImageCount = mSwapMinImageCount + 1;

    if(surface->GetCapabilities().maxImageCount > 0 && mSwapImageCount > surface->GetCapabilities().maxImageCount)
    {
        mSwapImageCount = surface->GetCapabilities().maxImageCount;
    }

    vk::SwapchainCreateInfoKHR createInfo;

    CreateSwapInfo(createInfo, surface);
    SetQueueFamilies(pDevice, createInfo);

    try {
        mSwapChain = std::make_unique<vk::raii::SwapchainKHR>(*lDevice->GetLogicalDevice(), createInfo);
        mSwapChainImages = mSwapChain->getImages();
        std::cout << "Created Swapchain\n";

        return true;

    } catch (const vk::SystemError& err) {
        std::cerr << "Failed to Create Swapchain Error: " << err.what() << "\n";

        return false;
    }
}

void VulkanSwapChain::CreateSwapInfo(vk::SwapchainCreateInfoKHR& info, const std::unique_ptr<VulkanSurface>& surface)
{
    info.setFlags(vk::SwapchainCreateFlagsKHR());
    info.setSurface(*surface->GetSurface());
    info.setMinImageCount(mSwapMinImageCount);
    info.setImageFormat(mSwapChainSurfaceFormat.format);
    info.setImageColorSpace(mSwapChainSurfaceFormat.colorSpace);
    info.setImageExtent(mSwapChainExtent);
    info.setImageArrayLayers(1);
    info.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
    info.setImageSharingMode(vk::SharingMode::eExclusive);
    info.setPreTransform(surface->GetCapabilities().currentTransform);
    info.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque);
    info.setPresentMode(surface->GetChosenPresentMode());
    info.setClipped(true);
    info.setOldSwapchain(nullptr);
}

void VulkanSwapChain::SetQueueFamilies(const std::unique_ptr<VulkanPhysicalDevice>& pDevice, vk::SwapchainCreateInfoKHR& info){

    uint32_t graphicsIndex = 0;
    uint32_t presentIndex = 0;

    for(CustomVKStructs::VkQueueFamilies family : pDevice->GetFamilies())
    {
        if(family.familyType == CustomVKStructs::RequiredVkFamilies::Graphics)
        {
            graphicsIndex = family.familyIndex;
        }

        if(family.familyType == CustomVKStructs::RequiredVkFamilies::Present)
        {
            presentIndex = family.familyIndex;
        }
    }

    uint32_t queueFamilyIndices[] = {graphicsIndex, presentIndex};

    if(graphicsIndex != presentIndex){
        info.setImageSharingMode(vk::SharingMode::eConcurrent);
        info.setQueueFamilyIndexCount(2);
        info.setPQueueFamilyIndices(queueFamilyIndices);
        std::cout << "Updated to Concurrent sharing mode\n";
    }

}

bool VulkanSwapChain::CreateImageViews(const std::unique_ptr<VulkanLogicalDevice>& lDevice){

    mSwapChainImageViews.clear();

    vk::ImageViewCreateInfo imageViewInfo;

    imageViewInfo.setViewType(vk::ImageViewType::e2D);
    imageViewInfo.setFormat(mSwapChainSurfaceFormat.format);
    imageViewInfo.setSubresourceRange({vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});

    try {

        for (const vk::Image& image : mSwapChainImages) {
            imageViewInfo.setImage(image);
            mSwapChainImageViews.emplace_back(std::make_unique<vk::raii::ImageView>(*lDevice->GetLogicalDevice(), imageViewInfo));
        }

        std::cout << "Created Image Views\n";

        return true;
    } catch (const vk::SystemError& err) {
        std::cerr << "Failed to Create Image Views Error: " << err.what() << "\n";
        return false;
    }
}

bool VulkanSwapChain::RecreateSwapChain(GLFWwindow* window,const std::unique_ptr<VulkanSurface>& surface,
                                 const std::unique_ptr<VulkanPhysicalDevice>& pDevice, const std::unique_ptr<VulkanLogicalDevice>& lDevice){
    int width = 0, height = 0;

    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    lDevice->GetLogicalDevice()->waitIdle();

    Cleanup();

    if(!CreateSwapchain(window, surface, pDevice, lDevice)) { return false;}
    if(!CreateImageViews(lDevice)) { return false; }

    std::cout << "Swap Chain Recreated\n";

    return true;
}

void VulkanSwapChain::Cleanup(){
    mSwapChainImageViews.clear();
    mSwapChain.reset();
}
