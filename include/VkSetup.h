//
// Created by wakiaki on 1/13/26.
//

#ifndef VULKANGAMES2DBASE_VKSETUP_H
#define VULKANGAMES2DBASE_VKSETUP_H
#include "CustomPD.h"


class VkSetup
{
public:
    VkSetup();
    ~VkSetup();

    bool InitVulkan();

private:

    bool CreateInstance();
    bool CheckExtensions(const vk::raii::Context& vContext, const uint32_t& extensionCount, const char**& extensions);

    vk::raii::Instance mVulkanInstance = nullptr;
    CustomPD mPhysicalDevice;
};


#endif //VULKANGAMES2DBASE_VKSETUP_H