#include "../include/CustomIV.h"
#include "vulkan/vulkan.hpp"
#include <memory>
#include <vulkan/vulkan_raii.hpp>

bool CustomIV::CreateImageViews(const std::unique_ptr<CustomSC>& swapchain, const std::unique_ptr<CustomLD>& lDevice) {

    mSwapChainImageViews = std::make_unique<std::vector<vk::raii::ImageView>>();

    mSwapChainImageViews->clear();

    vk::ImageViewCreateInfo createInfo;

    createInfo.setViewType(vk::ImageViewType::e2D);
    createInfo.setFormat(swapchain->GetFormat().format);
    createInfo.setSubresourceRange({vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});

    try {

        for(vk::Image imageInfo : swapchain->GetImages())
        {
            createInfo.image = imageInfo;
            mSwapChainImageViews->emplace_back(*lDevice->GetLogicalDevice(), createInfo);
        }

        std::cout << "Successfully created swapchain Images\n";

        return true;

    } catch (const vk::SystemError& err) {
        std::cerr << "Failed to Create Image Views\n";

        return false;
    }
}
