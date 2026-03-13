//
// Created by wakiaki on 1/13/26.
//

#include "../include/VulkanInstance.h"


bool VulkanInstance::InitVulkan()
{
    if (!CreateInstance()) { return false; }

    return true;
}

bool VulkanInstance::CreateInstance()
{
    try {
        vk::raii::Context context;
        constexpr vk::ApplicationInfo gameInfo("Game Name", VK_MAKE_VERSION(1, 0, 0), "Game Engine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_3);

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        mInstanceExtensions = std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);

    //#ifdef __APPLE__
    //    std::vector<const char*> allExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    //    allExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    //    glfwExtensionCount = static_cast<uint32_t>(allExtensions.size());
    //    glfwExtensions = allExtensions.data();
    //#endif

        auto availableExtensions = context.enumerateInstanceExtensionProperties();
        std::cout << "Available Instance Extensions: " << availableExtensions.size() << "\n";

        mInstanceExtensions.emplace_back("VK_KHR_get_physical_device_properties2");
        mInstanceExtensions.emplace_back("VK_EXT_debug_utils");

        std::array<const char*, 1> validationLayers = { "VK_LAYER_KHRONOS_validation" };

        //for (const auto& ext : availableExtensions) {
        //   std::cout << "  " << ext.extensionName << " v" << ext.specVersion << "\n";
        //}

        for(const char* ext : mInstanceExtensions)
        {
            std::cout << "Extension : " << ext << " added\n";
        }

        if (!CheckExtensions(context, mInstanceExtensions.size(), mInstanceExtensions.data())) { return false; }

        vk::InstanceCreateInfo createInfo;
        createInfo.setFlags({});
        createInfo.setPApplicationInfo(&gameInfo);
        createInfo.setEnabledLayerCount(1);
        createInfo.setPpEnabledLayerNames(validationLayers.data());
        createInfo.setEnabledExtensionCount(static_cast<uint32_t>(mInstanceExtensions.size()));
        createInfo.setPpEnabledExtensionNames(mInstanceExtensions.data());

        mVulkanInstance = std::make_unique<vk::raii::Instance>(context, createInfo);
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

bool VulkanInstance::CheckExtensions(const vk::raii::Context& vContext, const uint32_t& extensionCount, const char** extensions) {

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

void VulkanInstance::Cleanup(){
    mVulkanInstance.reset();
}
