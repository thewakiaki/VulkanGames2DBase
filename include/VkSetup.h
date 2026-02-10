//
// Created by wakiaki on 1/13/26.
//

#ifndef VULKANGAMES2DBASE_VKSETUP_H
#define VULKANGAMES2DBASE_VKSETUP_H
#include "CustomPD.h"


class VkSetup
{
public:
    VkSetup() = default;
    ~VkSetup() {};

    bool InitVulkan();

private:

    bool CreateInstance();
    bool CheckExtensions(const vk::raii::Context& vContext, const uint32_t& extensionCount, const char**& extensions);

    std::unique_ptr<vk::raii::Instance> mVulkanInstance;
    vk::raii::Context mContext;
    std::unique_ptr<CustomPD> mPhysicalDevice;
};


#endif //VULKANGAMES2DBASE_VKSETUP_H
