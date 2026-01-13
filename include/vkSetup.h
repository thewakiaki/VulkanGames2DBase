//
// Created by wakiaki on 1/13/26.
//

#ifndef VULKANGAMES2DBASE_VKSETUP_H
#define VULKANGAMES2DBASE_VKSETUP_H
#include <unordered_map>


class vkSetup
{
public:
    vkSetup() = default;
    ~vkSetup() = default;

    bool InitVulkan();

private:

    bool CreateInstance();
    bool CheckExtensions(const vk::raii::Context& vContext, const uint32_t& extensionCount, const char**& extensions);
    bool PickPhysicalDevice(vk::raii::PhysicalDevice pDevice);

    vk::raii::Instance mVulkanInstance = nullptr;
    vk::raii::PhysicalDevice mPhysicalDevice = nullptr;

    
};


#endif //VULKANGAMES2DBASE_VKSETUP_H