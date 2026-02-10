#ifndef CUSTOMLD_H
#define CUSTOMLD_H

#include "CustomPD.h"

class CustomLD{
public :
    CustomLD() = default;
    ~CustomLD();

    bool CreateDeviceInfo(const CustomPD& device);

private :
    std::unique_ptr<vk::raii::Device> mLogicalDevice;
    std::unique_ptr<vk::raii::Queue> mGraphicsQueue;

    std::vector<const char*> mDeviceExtensions;

    uint32_t mGraphicsFamily = 0;
};

#endif //CUSTOMLD_H
