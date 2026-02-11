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

#include "CustomSurface.h"
#include "CustomVkStructs.h"

class CustomPD {
public:
    CustomPD() = default;
    ~CustomPD() {};

    bool SetUpPhysicalDevice(const std::unique_ptr<vk::raii::Instance>& vk_instance, const std::unique_ptr<CustomSurface>& surface);

    const std::unique_ptr<vk::raii::PhysicalDevice>& GetPhysicalDevice() const { return mPhysicalDevice; }

    inline uint32_t GetGraphicsFamilyIndex() const { return mGraphicsFamilyIndex; }
    inline uint32_t GetPresentFamilyIndex() const { return mPresentFamilyIndex; }

private:
    void ScoreDevice(vk::raii::PhysicalDevice device);
    void GetMostSuitableDevice();

    bool SelectPhysicalDevice(const std::unique_ptr<vk::raii::Instance>& vk_instance);
    bool DeviceTypeSuitable(const vk::raii::PhysicalDevice& device);
    bool DeviceFeaturesSuitable(const std::unique_ptr<vk::raii::PhysicalDevice>& device);
    bool FindQueueFamilies(const std::unique_ptr<CustomSurface>& surface);

    std::unique_ptr<vk::raii::PhysicalDevice> mPhysicalDevice;

    std::vector<CustomVKStructs::PhysicalDeviceScore> mDeviceScores;

    bool mGraphicsQueueFound = false;
    bool mPresentQueueFound = false;

    uint32_t mGraphicsFamilyIndex = -1;
    uint32_t mPresentFamilyIndex = -1;
};


#endif //VULKANGAMES2DBASE_PHYISICALDEVICE_H
