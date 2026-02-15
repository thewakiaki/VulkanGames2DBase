#ifndef CUSTOMLD_H
#define CUSTOMLD_H

#include "CustomPD.h"

class CustomLD{
public :
    CustomLD() = default;
    ~CustomLD();

    bool CreateLogicalDevice(const CustomPD& device);

    const std::unique_ptr<vk::raii::Device>& GetLogicalDevice() const { return mLogicalDevice; }

private :
    std::unique_ptr<vk::raii::Device> mLogicalDevice;
    std::unique_ptr<vk::raii::Queue> mGraphicsQueue;
    std::unique_ptr<vk::raii::Queue> mPresentQueue;

    std::vector<const char*> mDeviceExtensions;

    uint32_t mGraphicsFamily = 0;
};

#endif //CUSTOMLD_H
