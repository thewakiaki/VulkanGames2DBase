#include "CustomSM.h"
#include "CustomLD.h"
#include "FileUtility.h"


bool CustomSM::CreateShaderModule(const CustomLD& lDevice, const char* fileName){

    mShaderCode = FileUtility::ParseFile(fileName);

    vk::ShaderModuleCreateInfo createInfo;

    SetCreateInfo(createInfo);

    try {
        mShaderModule = std::make_unique<vk::raii::ShaderModule>(*lDevice.GetLogicalDevice(), createInfo);

        std::cout << "Shader module for " << fileName << " successfully created\n";
    } catch (const vk::SystemError& err) {

        std::cerr << "Failed to create shader module for " << fileName << " Error: " << err.what() << "\n";
    }

    return true;
}

void CustomSM::SetCreateInfo(vk::ShaderModuleCreateInfo& info) {
    info.setCodeSize(mShaderCode.size() * sizeof(char));
    info.setPCode(reinterpret_cast<const uint32_t*>(mShaderCode.data()));
}

void CustomSM::Cleanup(){
    mShaderModule.reset();
}
