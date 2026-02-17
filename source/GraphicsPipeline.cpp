#include "../include/GraphicsPipeline.h"
#include "vulkan/vulkan.hpp"
#include <memory>



bool GraphicsPipeline::SetupShaders(){

    mVertShader = std::make_unique<CustomSM>();
    mFragShader = std::make_unique<CustomSM>();

    if(!mVertShader->CreateShaderModule(mLogicalDevice, "../shaders/triangle.vert.spv"))
    {
        std::cerr << "Failed to create vertex shader module\n";
        return false;
    };

    std::cout << "Succesfully created vertex shader module\n";

    if(!mFragShader->CreateShaderModule(mLogicalDevice, "../shaders/triangle.frag.spv"))
    {
        std::cerr << "Failed to create fragment shader module\n";
        return false;
    };

    std::cout << "Succesfully created fragment shader module\n";

    return true;
}

bool GraphicsPipeline::CreatePipeline(){

    vk::PipelineShaderStageCreateInfo vertCreateInfo;
    vk::PipelineShaderStageCreateInfo fragCreateInfo;

    vertCreateInfo.setStage(vk::ShaderStageFlagBits::eVertex);
    vertCreateInfo.setModule(*mVertShader->GetShaderModule());
    vertCreateInfo.setPName("main");

    fragCreateInfo.setStage(vk::ShaderStageFlagBits::eFragment);
    fragCreateInfo.setModule(*mVertShader->GetShaderModule());
    fragCreateInfo.setPName("main");

    vk::PipelineShaderStageCreateInfo shaderStages[] = {vertCreateInfo, fragCreateInfo};

    return true;
}
