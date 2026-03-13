#ifndef CUSTOM_SM_H
#define CUSTOM_SM_H


#include "VulkanLogicalDevice.h"

class VulkanShaderModule {

public:
    bool CreateShaderModule(const VulkanLogicalDevice& lDevice, const char* fileName);

    void Cleanup();

    [[nodiscard]] const std::unique_ptr<vk::raii::ShaderModule>& GetShaderModule() const { return mShaderModule; }


private:
    void SetCreateInfo(vk::ShaderModuleCreateInfo& info);

    std::unique_ptr<vk::raii::ShaderModule> mShaderModule;

    std::vector<char> mShaderCode;

};

#endif // CUSTOM_SM_H
