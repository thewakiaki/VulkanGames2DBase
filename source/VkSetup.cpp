//
// Created by wakiaki on 1/13/26.
//

#include "../include/VkSetup.h"


bool VkSetup::InitVulkan()
{
    if (!CreateInstance()) { return false; }

    return true;
}

bool VkSetup::CreateInstance()
{
    try {

        constexpr vk::ApplicationInfo gameInfo("Game Name", VK_MAKE_VERSION(1, 0, 0), "Game Engine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_0);

        uint32_t glfwExtensionCount = 0;
        auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    #ifdef __APPLE__
        std::vector<const char*> allExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        allExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        glfwExtensionCount = static_cast<uint32_t>(allExtensions.size());
        glfwExtensions = allExtensions.data();
    #endif


        if (!CheckExtensions(mContext, glfwExtensionCount, glfwExtensions)) { return false; }

        vk::InstanceCreateInfo create_info({}, &gameInfo, 0, nullptr, glfwExtensionCount, glfwExtensions);


        mVulkanInstance = std::make_unique<vk::raii::Instance>(mContext, create_info);
        if(!mVulkanInstance) {
            std::cerr << "Failed to create VK_INSTANCE\n";
            return false;
        }

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

bool VkSetup::CheckExtensions(const vk::raii::Context& vContext, const uint32_t& extensionCount, const char**& extensions) {

    auto extensionProperties = vContext.enumerateInstanceExtensionProperties();
    bool extensionFound = false;


    for (uint32_t i = 0; i < extensionCount; ++i) {
        extensionFound = false;

        for (const auto& property : extensionProperties) {
            if (strcmp(property.extensionName, extensions[i]) == 0) {
                extensionFound = true;
                break;
            }
        }

        if (!extensionFound) {
            std::cerr << "Warning: extension " << extensions[i] << " not found\n";
            return false;
        }

    }

    return true;
}
