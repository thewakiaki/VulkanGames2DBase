//
// Created by wakiaki on 1/14/26.
//

#ifndef VULKANGAMES2DBASE_PHYISICALDEVICE_H
#define VULKANGAMES2DBASE_PHYISICALDEVICE_H

#include <vulkan/vulkan_core.h>

#include "CustomVkStructs.h"

class CustomPD {
public:
    CustomPD();
    ~CustomPD() {};

    void SelectPhysicalDevice(const vk::raii::Instance& vk_instance);

    vk::raii::PhysicalDevice& getPhysicalDevice() { return mPhysicalDevice; }

private:
    void ScoreDevice(const vk::raii::PhysicalDevice& device);
    void GetMostSuitableDevice();

    bool DeviceTypeSuitable(const vk::raii::PhysicalDevice& device);
    bool DeviceFeaturesSuitable(const vk::raii::PhysicalDevice& device);

    vk::raii::PhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;

    std::vector<CustomVKStructs::PhysicalDeviceScore> deviceScores;
};


#endif //VULKANGAMES2DBASE_PHYISICALDEVICE_H
