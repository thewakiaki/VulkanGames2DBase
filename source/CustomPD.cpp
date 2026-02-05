//
// Created by wakiaki on 1/14/26.
//
#include <cstddef>
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_raii.hpp>

#include "../include/CustomPD.h"
#include "CustomVkStructs.h"
#include "vulkan/vulkan.hpp"

CustomPD::CustomPD()
{

}

bool CustomPD::SetUpPhysicalDevice(const vk::raii::Instance& vk_instance){

    return SelectPhysicalDevice(vk_instance) && (FindQueueFamilies() != mNoGraphicsQueue);
}

bool CustomPD::SelectPhysicalDevice(const vk::raii::Instance& vk_instance) {

    std::vector<vk::raii::PhysicalDevice> devices = vk_instance.enumeratePhysicalDevices();

    if (devices.empty()) {
        std::cerr << "No physical device with Vulkan support found\n";
        return false;
    }

    std::cout << "Physical device found\n";

    for (const vk::raii::PhysicalDevice& device : devices) {
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
    std::cout << "Most Suitable Device: " << mPhysicalDevice.getProperties().deviceName << "\n";
    return true;
}

void CustomPD::ScoreDevice(const vk::raii::PhysicalDevice &device) {

    int score = 0;

    vk::PhysicalDeviceFeatures availableFeatures = device.getFeatures();

    ///ADD FEATURES YOU WANT HERE AND SCORE BASED ON YOUR IMPORTANCE TO THE GAME
    if(availableFeatures.samplerAnisotropy) { score += 17; }
    if(availableFeatures.fillModeNonSolid) { score += 10; }
    if(availableFeatures.geometryShader) { score += 20; }
    if(availableFeatures.wideLines) { score += 5; }
    if(availableFeatures.textureCompressionBC) { score += 8; }

    std::cout << "Device : " << device.getProperties().deviceName << " Scored: " << score << "\n";

    deviceScores.emplace_back(CustomVKStructs::PhysicalDeviceScore(score, device));
}

void CustomPD::GetMostSuitableDevice() {

    CustomVKStructs::PhysicalDeviceScore bestDevice = deviceScores[0];

    for(size_t device = 1; device < deviceScores.size(); ++device)
    {
        const CustomVKStructs::PhysicalDeviceScore& current = deviceScores[device];

        if(bestDevice.score < current.score)
        {
            bestDevice = current;
        }

        if(bestDevice.score == current.score)
        {
            bool type1 = bestDevice.physical_device.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu;
            bool type2 = current.physical_device.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu;

            if(type1 < type2) { bestDevice = deviceScores[device]; }
        }

    }

    mPhysicalDevice = bestDevice.physical_device;
}

bool CustomPD::DeviceTypeSuitable(const vk::raii::PhysicalDevice& device) {

    vk::PhysicalDeviceProperties deviceProperties = device.getProperties();

    return deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu || deviceProperties.deviceType == vk::PhysicalDeviceType::eIntegratedGpu;
}

uint32_t CustomPD::FindQueueFamilies(){

    const std::vector<vk::QueueFamilyProperties>& deviceQueueFamilies = mPhysicalDevice.getQueueFamilyProperties();

    for(uint32_t queue_index = 0; queue_index < deviceQueueFamilies.size(); ++queue_index)
    {
        if(deviceQueueFamilies[queue_index].queueFlags & vk::QueueFlagBits::eGraphics)
        {
            std::cout << "Graphics Queue Families found\n";
            return queue_index;
        }
    }

    std::cerr << "No required Queue Families found\n";
    return mNoGraphicsQueue;
}
