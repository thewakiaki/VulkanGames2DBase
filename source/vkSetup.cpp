//
// Created by wakiaki on 1/13/26.
//

#include "../include/vkSetup.h"
#include <algorithm>

bool vkSetup::InitVulkan()
{
    if (!CreateInstance()) { return false; }

    return true;
}

bool vkSetup::CreateInstance()
{
    try {
        vk::raii::Context context;


        constexpr vk::ApplicationInfo game_info
        {
            vk::StructureType::eApplicationInfo,
            nullptr,
            "Game Info",
            VK_MAKE_VERSION(1, 0, 0),
            "No Engine",
            VK_MAKE_VERSION(1, 0, 0),
            vk::ApiVersion14
        };

        uint32_t glfwExtensionCount = 0;
        auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    #ifdef __APPLE__
        std::vector<const char*> allExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        allExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        glfwExtensionCount = static_cast<uint32_t>(allExtensions.size());
        glfwExtensions = allExtensions.data();
    #endif


        if (!CheckExtensions(context, glfwExtensionCount, glfwExtensions)) { return false; }

        vk::InstanceCreateInfo create_info;
        create_info.setPApplicationInfo(&game_info);
        create_info.enabledExtensionCount = glfwExtensionCount;
        create_info.ppEnabledExtensionNames = glfwExtensions;

        mVulkanInstance = vk::raii::Instance(context, create_info);
        return true;
    }
    catch (const vk::SystemError& err)
    {
        std::cerr << "Vulkan error: " << err.what() << "\n";
        return false;
    }
    catch (const std::exception& err)
    {
        std::cerr << "Error: " << err.what() << "\n";
        return false;
    }


}

bool vkSetup::CheckExtensions(const vk::raii::Context& vContext, const uint32_t& extensionCount, const char**& extensions)
{
    auto extensionProperties = vContext.enumerateInstanceExtensionProperties();
    bool extensionFound = false;


    for (uint32_t i = 0; i < extensionCount; ++i)
    {
        extensionFound = false;

        for (const auto& property : extensionProperties)
        {
            if (strcmp(property.extensionName, extensions[i]) == 0)
            {
                extensionFound = true;
                break;
            }
        }

        if (!extensionFound)
        {
            std::cerr << "Warning: extension " << extensions[i] << " not found\n";
            return false;
        }

    }

    return true;
}

bool vkSetup::PickPhysicalDevice(vk::raii::PhysicalDevice pDevice)
{
    std::vector<vk::raii::PhysicalDevice> devices = mVulkanInstance.enumeratePhysicalDevices();

    if (devices.empty())
    {
        std::cerr << "No physical device found\n";
        return false;
    }

    for (const vk::raii::PhysicalDevice& device : devices)
    {
        pDevice = device;
        break;
    }

    return true;
}
