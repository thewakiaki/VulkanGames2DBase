#ifndef CUSTOM_SM_H
#define CUSTOM_SM_H


#include "CustomLD.h"

class CustomSM {

public:
    bool CreateShaderModule(const CustomLD& lDevice, const char* fileName);

    inline const std::unique_ptr<vk::raii::ShaderModule>& GetShaderModule() const { return mShaderModule; }


private:
    void SetCreateInfo(vk::ShaderModuleCreateInfo& info);

    std::unique_ptr<vk::raii::ShaderModule> mShaderModule;

    std::vector<char> mShaderCode;

};

#endif // CUSTOM_SM_H
