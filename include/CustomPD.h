//
// Created by wakiaki on 1/14/26.
//

#ifndef VULKANGAMES2DBASE_PHYISICALDEVICE_H
#define VULKANGAMES2DBASE_PHYISICALDEVICE_H

#include <SDL3/SDL_stdinc.h>
#include <cstdint>
#include <limits>
#include <vulkan/vulkan_core.h>

#include "CustomVkStructs.h"

class CustomPD {
public:
    CustomPD();
    ~CustomPD() {};

    bool SetUpPhysicalDevice(const vk::raii::Instance& vk_instance);

    vk::raii::PhysicalDevice& getPhysicalDevice() { return mPhysicalDevice; }

private:
    void ScoreDevice(const vk::raii::PhysicalDevice& device);
    void GetMostSuitableDevice();

    bool SelectPhysicalDevice(const vk::raii::Instance& vk_instance);
    bool DeviceTypeSuitable(const vk::raii::PhysicalDevice& device);
    bool DeviceFeaturesSuitable(const vk::raii::PhysicalDevice& device);


    uint32_t FindQueueFamilies();

    vk::raii::PhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;

    std::vector<CustomVKStructs::PhysicalDeviceScore> deviceScores;

    const uint32_t mNoGraphicsQueue = std::numeric_limits<uint32_t>::max();
};


#endif //VULKANGAMES2DBASE_PHYISICALDEVICE_H
