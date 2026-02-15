#include "../include/CustomLD.h"
#include "../include/CustomPD.h"
#include "CustomVkStructs.h"
#include "vulkan/vulkan.hpp"
#include <cstdint>
#include <vector>

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

    mDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    const std::vector<CustomVKStructs::VkQueueFamilies> queueFamilies = device.GetFamilies();

    vk::DeviceCreateInfo deviceCreateInfo;
    vk::DeviceQueueCreateInfo deviceQueueInfos[queueFamilies.size()];

    //to enable more vk features set up feature chain and set pnext to point to this

    //Always set graphics family first (int)CustomVKStructs::RequiredVkFamilies::Graphics this should always be 0
    //as graphics is always first family to look for
    deviceQueueInfos[0].setQueueFamilyIndex(device.GetFamilies()[(int)CustomVKStructs::RequiredVkFamilies::Graphics].familyIndex);
    deviceQueueInfos[0].setQueueCount(1);
    deviceQueueInfos[0].setPQueuePriorities(&queuePriority);

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
