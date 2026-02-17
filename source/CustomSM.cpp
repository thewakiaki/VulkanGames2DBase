#include "CustomSM.h"
#include "CustomLD.h"
#include "FileUtility.h"
#include "vulkan/vulkan.hpp"
#include <memory>
#include <vulkan/vulkan_raii.hpp>


bool CustomSM::CreateShaderModule(const CustomLD& lDevice, const char* fileName){

    mShaderCode = FileUtility::ParseFile(fileName);

    vk::ShaderModuleCreateInfo createInfo;

    SetCreateInfo(createInfo);

    mShaderModule = std::make_unique<vk::raii::ShaderModule>(*lDevice.GetLogicalDevice(), createInfo);

    if(!mShaderModule)
    {
        std::cerr << "Failed to create module\n";
        return false;
    }

    return true;
}

void CustomSM::SetCreateInfo(vk::ShaderModuleCreateInfo& info) {
    info.setCodeSize(mShaderCode.size() * sizeof(char));
    info.setPCode(reinterpret_cast<const uint32_t*>(mShaderCode.data()));
}
