//
// Created by wakiaki on 1/14/26.
//

#ifndef VULKANGAMES2DBASE_PHYISICALDEVICE_H
#define VULKANGAMES2DBASE_PHYISICALDEVICE_H

#include <SDL3/SDL_stdinc.h>
#include <cstdint>
#include <limits>
#include <memory>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_raii.hpp>

#include "CustomVkStructs.h"

class CustomPD {
public:
    CustomPD() = default;
    ~CustomPD() {};

    bool SetUpPhysicalDevice(const std::unique_ptr<vk::raii::Instance>& vk_instance);


    const std::unique_ptr<vk::raii::PhysicalDevice>& GetPhysicalDevice() const { return mPhysicalDevice; }

    inline uint32_t GetGraphicsFamilyIndex() const { return mGraphicsFamilyIndex; }

private:
    void ScoreDevice(vk::raii::PhysicalDevice device);
    void GetMostSuitableDevice();

    bool SelectPhysicalDevice(const std::unique_ptr<vk::raii::Instance>& vk_instance);
    bool DeviceTypeSuitable(const vk::raii::PhysicalDevice& device);
    bool DeviceFeaturesSuitable(const std::unique_ptr<vk::raii::PhysicalDevice>& device);


    uint32_t FindQueueFamilies();

    std::unique_ptr<vk::raii::PhysicalDevice> mPhysicalDevice;

    std::vector<CustomVKStructs::PhysicalDeviceScore> mDeviceScores;

    const uint32_t mNoGraphicsQueue = std::numeric_limits<uint32_t>::max();

    uint32_t mGraphicsFamilyIndex = 0;
};


#endif //VULKANGAMES2DBASE_PHYISICALDEVICE_H
