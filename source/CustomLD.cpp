#include "../include/CustomLD.h"
#include "CustomVkStructs.h"
#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan_core.h>


CustomLD::~CustomLD(){

}

bool CustomLD::CreateLogicalDevice(const CustomPD& device){

    const uint32_t& graphicsFamily = device.GetFamilies()[(int)CustomVKStructs::RequiredVkFamilies::Graphics].familyIndex;
    const uint32_t& presentFamily =  device.GetFamilies()[(int)CustomVKStructs::RequiredVkFamilies::Present].familyIndex;

    vk::DeviceQueueCreateInfo queueCreateInfo;
    const float queuePriority = 0.5f;
    queueCreateInfo.setQueueFamilyIndex(graphicsFamily);
    queueCreateInfo.setQueueCount(1);
    queueCreateInfo.setPQueuePriorities(&queuePriority);


    const std::vector<CustomVKStructs::VkQueueFamilies> queueFamilies = device.GetFamilies();

    vk::DeviceCreateInfo deviceCreateInfo;

    std::vector<vk::DeviceQueueCreateInfo> deviceQueueInfos;

    mDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                          VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
                          VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
                          VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME,
                          VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
                          VK_KHR_MULTIVIEW_EXTENSION_NAME,
                          VK_KHR_MAINTENANCE2_EXTENSION_NAME};

    //to enable more vk features set up feature chain and set pnext to point to this - need to be set up in reverse order
    //Features at the bottom of the dependcy tree first
    vk::PhysicalDeviceSynchronization2FeaturesKHR sync2Features{};
    sync2Features.setSynchronization2(true);

    vk::PhysicalDeviceDynamicRenderingFeaturesKHR dynamicFeatures{};
    dynamicFeatures.setDynamicRendering(true);
    dynamicFeatures.setPNext(&sync2Features);

    vk::PhysicalDeviceVulkan11Features vk11Featrues{};
    vk11Featrues.setShaderDrawParameters(true);
    vk11Featrues.setPNext(&dynamicFeatures);

    vk::PhysicalDeviceFeatures2 features{};
    features.setPNext(&vk11Featrues);

    deviceCreateInfo.setPNext(&features);

    vk::DeviceQueueCreateInfo graphicsQueueInfo{};
    graphicsQueueInfo.setQueueFamilyIndex(device.GetFamilies()[(int)CustomVKStructs::RequiredVkFamilies::Graphics].familyIndex);
    graphicsQueueInfo.setQueueCount(1);
    graphicsQueueInfo.setPQueuePriorities(&queuePriority);
    deviceQueueInfos.emplace_back(graphicsQueueInfo);

    int queuesAdded = 1;

    //Set other families
    for(int info = 1; info < device.GetFamilies().size(); ++info)
    {
        if(device.GetFamilies()[info].familyIndex != deviceQueueInfos[0].queueFamilyIndex)
        {
            deviceQueueInfos[queuesAdded].setQueueFamilyIndex(device.GetFamilies()[info].familyIndex);
            deviceQueueInfos[queuesAdded].setQueueCount(1);
            deviceQueueInfos[queuesAdded].setPQueuePriorities(&queuePriority);
            ++queuesAdded;
        }
    }

    deviceCreateInfo.enabledExtensionCount = mDeviceExtensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = mDeviceExtensions.data();
    deviceCreateInfo.setQueueCreateInfoCount(queuesAdded);
    deviceCreateInfo.setPQueueCreateInfos(deviceQueueInfos.data());

    try {
        mLogicalDevice = std::make_unique<vk::raii::Device>(*device.GetPhysicalDevice(), deviceCreateInfo);
        std::cout << "Logical Device Created\n";

        mGraphicsQueue = std::make_unique<vk::raii::Queue>(*mLogicalDevice, graphicsFamily, 0);
        mPresentQueue = std::make_unique<vk::raii::Queue>(*mLogicalDevice, presentFamily, 0);

        return true;
    } catch (const vk::SystemError& err) {
        std::cerr << "Failed to Create Device\n";
        return false;
    }

}
