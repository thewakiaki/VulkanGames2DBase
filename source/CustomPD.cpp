//
// Created by wakiaki on 1/14/26.
//
#include "../include/CustomPD.h"
#include "CustomVkStructs.h"
#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan_core.h>

bool CustomPD::SetUpPhysicalDevice(const std::unique_ptr<vk::raii::Instance>& vk_instance, const std::unique_ptr<CustomSurface>& surface){

    return SelectPhysicalDevice(vk_instance) && (FindQueueFamilies(surface));
}

bool CustomPD::SelectPhysicalDevice(const std::unique_ptr<vk::raii::Instance>& vk_instance) {

    std::vector<vk::raii::PhysicalDevice> devices = vk_instance->enumeratePhysicalDevices();

    if (devices.empty()) {
        std::cerr << "No physical device with Vulkan support found\n";
        return false;
    }

    std::cout << "Physical device found\n";

    for (vk::raii::PhysicalDevice device : devices) {
        int score = 0;

        vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
        vk::PhysicalDeviceFeatures deviceFeatures = device.getFeatures();

        if (!DeviceTypeSuitable(device)) {
            std::cerr << "Physical device type not suitable\n";
            continue;
        }


        ScoreDevice(device);
    }


    GetMostSuitableDevice();
    std::cout << "Most Suitable Device: " << mPhysicalDevice->getProperties().deviceName << "\n";
    return true;
}

void CustomPD::ScoreDevice(vk::raii::PhysicalDevice device) {

    int score = 0;

    vk::PhysicalDeviceFeatures availableFeatures = device.getFeatures();

    ///ADD FEATURES YOU WANT HERE AND SCORE BASED ON YOUR IMPORTANCE TO THE GAME
    if(availableFeatures.samplerAnisotropy) { score += 17; }
    if(availableFeatures.fillModeNonSolid) { score += 10; }
    if(availableFeatures.geometryShader) { score += 20; }
    if(availableFeatures.wideLines) { score += 5; }
    if(availableFeatures.textureCompressionBC) { score += 8; }

    std::cout << "Device : " << device.getProperties().deviceName << " Scored: " << score << "\n";

    mDeviceScores.emplace_back(CustomVKStructs::PhysicalDeviceScore(score, std::make_unique<vk::raii::PhysicalDevice>(device)));
}

void CustomPD::GetMostSuitableDevice() {

     uint32_t bestDevice = 0;

    for(uint32_t device = 1; device < mDeviceScores.size(); ++device)
    {

        if(mDeviceScores[bestDevice].score < mDeviceScores[device].score)
        {
            bestDevice = device;
        }

        if(mDeviceScores[bestDevice].score == mDeviceScores[device].score)
        {
            bool type1 = mDeviceScores[bestDevice].physical_device->getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu;
            bool type2 = mDeviceScores[device].physical_device->getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu;

            if(type1 < type2) { bestDevice = device; }
        }

    }

    mPhysicalDevice = std::move(mDeviceScores[bestDevice].physical_device);
}

bool CustomPD::DeviceTypeSuitable(const vk::raii::PhysicalDevice& device) {

    vk::PhysicalDeviceProperties deviceProperties = device.getProperties();

    return deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu || deviceProperties.deviceType == vk::PhysicalDeviceType::eIntegratedGpu;
}

bool CustomPD::FindQueueFamilies(const std::unique_ptr<CustomSurface>& surface){

    if(!mPhysicalDevice)
    {
        std::cerr << "No Device Available\n";
        return false;
    }

    const std::vector<vk::QueueFamilyProperties>& deviceQueueFamilies = mPhysicalDevice->getQueueFamilyProperties();

    for(uint32_t queue_index = 0; queue_index < deviceQueueFamilies.size(); ++queue_index)
    {
        if(deviceQueueFamilies[queue_index].queueFlags & vk::QueueFlagBits::eGraphics)
        {
            std::cout << "Graphics Queue Families found\n";
            mGraphicsFamilyIndex = queue_index;
            mGraphicsQueueFound = true;
        }

        VkBool32 presentSupported = mPhysicalDevice->getSurfaceSupportKHR(queue_index, *surface->GetSurface());

        if(presentSupported)
        {
            std::cout << "Present Queue found\n";
            mPresentFamilyIndex = queue_index;
            mPresentQueueFound = true;
        }
    }

    if(!mGraphicsQueueFound)
    {
        std::cout << "No Graphics queue found\n";
    }

    if(!mPresentQueueFound)
    {
        std::cout << "No Present queue found\n";
    }

    return mGraphicsQueueFound && mPresentQueueFound;
}
