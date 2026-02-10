#include "../include/CustomLD.h"
#include "../include/CustomPD.h"

CustomLD::~CustomLD(){

}

bool CustomLD::CreateDeviceInfo(const CustomPD& device){
    vk::DeviceQueueCreateInfo queueCreateInfo;
    const float queuePriority = 1.0f;
    queueCreateInfo.setQueueFamilyIndex(device.GetGraphicsFamilyIndex());
    queueCreateInfo.setQueueCount(1);
    queueCreateInfo.setPQueuePriorities(&queuePriority);




    mDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    vk::DeviceCreateInfo deviceCreateInfo;

    //to enable more vk features set up feature chain and set pnext to point to this
    deviceCreateInfo.setPNext(nullptr);
    deviceCreateInfo.setQueueCreateInfoCount(1);
    deviceCreateInfo.setPQueueCreateInfos(&queueCreateInfo);
    deviceCreateInfo.setEnabledExtensionCount(mDeviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = mDeviceExtensions.data();

    try {
        mLogicalDevice = std::make_unique<vk::raii::Device>(*device.GetPhysicalDevice(), deviceCreateInfo);
        std::cout << "Logical Device Created\n";
        mGraphicsQueue = std::make_unique<vk::raii::Queue>(*mLogicalDevice, device.GetGraphicsFamilyIndex(), 0);

        return true;
    } catch (const vk::SystemError& err) {
        std::cerr << "Failed to Create Device\n";
        return false;
    }

}
