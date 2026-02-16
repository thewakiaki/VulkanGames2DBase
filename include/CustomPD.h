//
// Created by wakiaki on 1/14/26.
//

#ifndef VULKANGAMES2DBASE_PHYISICALDEVICE_H
#define VULKANGAMES2DBASE_PHYISICALDEVICE_H

#include "CustomSurface.h"
#include "CustomVkStructs.h"

class CustomPD {
public:
    CustomPD() = default;
    ~CustomPD() {};

    bool SetUpPhysicalDevice(const std::unique_ptr<vk::raii::Instance>& vk_instance, const std::unique_ptr<CustomSurface>& surface);

    const std::unique_ptr<vk::raii::PhysicalDevice>& GetPhysicalDevice() const { return mPhysicalDevice; }

    inline std::vector<CustomVKStructs::VkQueueFamilies> GetFamilies() const { return mQueueFamilies; }

    inline const vk::PhysicalDeviceFeatures2 GetDeviceFeatures() const { return mDeviceFeatures;}

private:
    void ScoreDevice(vk::raii::PhysicalDevice device);
    void GetMostSuitableDevice();
    void UpdateSurfaceDetails(const std::unique_ptr<CustomSurface>& surface);

    bool SelectPhysicalDevice(const std::unique_ptr<vk::raii::Instance>& vk_instance);
    bool DeviceTypeSuitable(const vk::raii::PhysicalDevice& device);
    bool DeviceFeaturesSuitable(const std::unique_ptr<vk::raii::PhysicalDevice>& device);
    bool FindQueueFamilies(const std::unique_ptr<CustomSurface>& surface);
    bool SetupVulkanDeviceFeatures();


    std::unique_ptr<vk::raii::PhysicalDevice> mPhysicalDevice;

    std::vector<CustomVKStructs::PhysicalDeviceScore> mDeviceScores;
    std::vector<CustomVKStructs::VkQueueFamilies> mQueueFamilies;

    vk::PhysicalDeviceFeatures2 mDeviceFeatures;
    vk::PhysicalDeviceVulkan13Features mVulkan13Features;
    vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT mDynamicStateFeatures;
};


#endif //VULKANGAMES2DBASE_PHYISICALDEVICE_H
