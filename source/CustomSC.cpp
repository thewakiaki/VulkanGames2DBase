#include "../include/CustomSC.h"
#include "CustomVkStructs.h"
#include "vulkan/vulkan.hpp"
#include <memory>
#include <vector>
#include <vulkan/vulkan_raii.hpp>

bool CustomSC::CreateSwapchain(GLFWwindow* window,const std::unique_ptr<CustomSurface>& surface,const std::unique_ptr<CustomPD>& pDevice, const std::unique_ptr<CustomLD>& lDevice){

    mSwapChainSurfaceFormat = surface->GetChosenFormat();
    mSwapChainExtent = surface->GetChosenExtent();

    mSwapMinImageCount = std::max(3u, surface->GetCapabilities().minImageCount);
    mSwapMinImageCount = ( surface->GetCapabilities().maxImageCount > 0 && mSwapMinImageCount > surface->GetCapabilities().maxImageCount) ? surface->GetCapabilities().maxImageCount : mSwapMinImageCount;

    std::cout << "Successfully created Swapchain\n";

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
        std::cerr << "Failed to Create Swapchain\n";

        return false;
    }
}

void CustomSC::CreateSwapInfo(vk::SwapchainCreateInfoKHR& info, const std::unique_ptr<CustomSurface>& surface)
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

void CustomSC::SetQueueFamilies(const std::unique_ptr<CustomPD>& pDevice, vk::SwapchainCreateInfoKHR& info){

    uint32_t graphicsIndex;
    uint32_t presentIndex;

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

    std::cout << "Concurrent mode not needed keeping exclusive mode\n";

}

bool CustomSC::CreateImageViews(const std::unique_ptr<CustomLD>& lDevice){

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
        std::cerr << "Failed to Create Image Views\n";
        return false;
    }
}
