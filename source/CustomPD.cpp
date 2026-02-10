//
// Created by wakiaki on 1/14/26.
//
#include "../include/CustomPD.h"
#include "CustomVkStructs.h"
#include "vulkan/vulkan.hpp"

bool CustomPD::SetUpPhysicalDevice(const std::unique_ptr<vk::raii::Instance>& vk_instance){

    return SelectPhysicalDevice(vk_instance) && (FindQueueFamilies() != mNoGraphicsQueue);
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

uint32_t CustomPD::FindQueueFamilies(){

    if(!mPhysicalDevice)
    {
        std::cerr << "No Device Available\n";
        return mNoGraphicsQueue;
    }

    const std::vector<vk::QueueFamilyProperties>& deviceQueueFamilies = mPhysicalDevice->getQueueFamilyProperties();

    for(uint32_t queue_index = 0; queue_index < deviceQueueFamilies.size(); ++queue_index)
    {
        if(deviceQueueFamilies[queue_index].queueFlags & vk::QueueFlagBits::eGraphics)
        {
            std::cout << "Graphics Queue Families found\n";
            mGraphicsFamilyIndex = queue_index;
            return mGraphicsFamilyIndex;
        }
    }

    std::cerr << "No required Queue Families found\n";
    return mNoGraphicsQueue;
}
