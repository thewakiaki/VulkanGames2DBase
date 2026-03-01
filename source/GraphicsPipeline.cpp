#include "../include/GraphicsPipeline.h"
#include "vulkan/vulkan.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_raii.hpp>

bool GraphicsPipeline::SetupShaders(){

    mVertShader = std::make_unique<CustomSM>();
    mFragShader = std::make_unique<CustomSM>();

    if(!mVertShader->CreateShaderModule(mLogicalDevice, "../shaders/slang.spv"))
    {
        std::cerr << "Failed to create vertex shader module\n";
        return false;
    };

    std::cout << "Succesfully created vertex shader module\n";

    if(!mFragShader->CreateShaderModule(mLogicalDevice, "../shaders/slang.spv"))
    {
        std::cerr << "Failed to create fragment shader module\n";
        return false;
    };

    std::cout << "Succesfully created fragment shader module\n";

    return true;
}

bool GraphicsPipeline::CreatePipeline(const std::unique_ptr<CustomSC>& swapchain){

    vk::PipelineShaderStageCreateInfo vertCreateInfo;
    vk::PipelineShaderStageCreateInfo fragCreateInfo;
    vk::PipelineDynamicStateCreateInfo dynamicInfo;
    vk::PipelineVertexInputStateCreateInfo vertInputInfo;
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    vk::PipelineViewportStateCreateInfo viewportStateInfo({}, 1, {}, 1);
    vk::PipelineRasterizationStateCreateInfo rasterCreateInfo;
    vk::PipelineMultisampleStateCreateInfo multisampleInfo;
    vk::PipelineColorBlendStateCreateInfo colorBlendInfo;
    vk::PipelineLayoutCreateInfo layoutCreatInfo;
    vk::PipelineRenderingCreateInfo renderInfo;
    vk::GraphicsPipelineCreateInfo pipelineInfo;

    SetPipelineVertShaderCreateInfo(vertCreateInfo, mVertShader);
    SetPipelineFragShaderCreateInfo(fragCreateInfo, mFragShader);

    vk::PipelineShaderStageCreateInfo shaderStages[] = {vertCreateInfo, fragCreateInfo};

    SetPipelineDynamicCreateInfo(dynamicInfo);
    SetPipelineVertInputCreateInfo(vertInputInfo);
    SetPipelineInputAssemblyCreateInfo(inputAssemblyInfo);
    SetPipelineRasterCreateInfo(rasterCreateInfo);
    SetPipelineColorBlendCreateInfo(colorBlendInfo);
    SetPiplineRenderCreateInfo(renderInfo, swapchain);
    SetPipelineLayoutCreateInfo(layoutCreatInfo);
    SetGraphicsPipilineCreateInfo(pipelineInfo, shaderStages, vertInputInfo, inputAssemblyInfo, viewportStateInfo, rasterCreateInfo, multisampleInfo, colorBlendInfo, dynamicInfo, renderInfo);

    try {
        mGraphicsPipeline = std::make_unique<vk::raii::Pipeline>(mLogicalDevice.GetLogicalDevice()->createGraphicsPipeline(nullptr, pipelineInfo, nullptr));
        std::cout << "Created Graphics Pipeline\n";

        return true;

    } catch (const vk::SystemError& err) {
        std::cerr << "Failed to Create Graphics Pipeline\n";

        return false;
    }
}

void GraphicsPipeline::SetPipelineVertShaderCreateInfo(vk::PipelineShaderStageCreateInfo& info, std::unique_ptr<CustomSM>& shader){

    info.setStage(vk::ShaderStageFlagBits::eVertex);
    info.setModule(*shader->GetShaderModule());

    info.setPName("vertMain");
}

void GraphicsPipeline::SetPipelineFragShaderCreateInfo(vk::PipelineShaderStageCreateInfo& info, std::unique_ptr<CustomSM>& shader){

    info.setStage(vk::ShaderStageFlagBits::eFragment);
    info.setModule(*shader->GetShaderModule());

    info.setPName("fragMain");

}

void GraphicsPipeline::SetPipelineDynamicCreateInfo(vk::PipelineDynamicStateCreateInfo& info){

    std::vector<vk::DynamicState> dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };

    info.setDynamicStates(dynamicStates);
}

void GraphicsPipeline::SetPipelineVertInputCreateInfo(vk::PipelineVertexInputStateCreateInfo& info){

    //glsl used so no buffer needed

    info.setVertexBindingDescriptions(nullptr);
    info.setVertexAttributeDescriptions(nullptr);
}

void GraphicsPipeline::SetPipelineInputAssemblyCreateInfo(vk::PipelineInputAssemblyStateCreateInfo& info){

    info.setTopology(vk::PrimitiveTopology::eTriangleList);
    info.setPrimitiveRestartEnable(vk::False);
}

void GraphicsPipeline::SetPipelineRasterCreateInfo(vk::PipelineRasterizationStateCreateInfo& info){

    info.setDepthClampEnable(vk::False);
    info.setRasterizerDiscardEnable(vk::False);
    info.setPolygonMode(vk::PolygonMode::eFill);
    info.setCullMode(vk::CullModeFlagBits::eNone);
    info.setFrontFace(vk::FrontFace::eCounterClockwise);
    info.setDepthBiasEnable(vk::False);
    info.setDepthBiasSlopeFactor(1.0f);
    info.setLineWidth(1.0f);
}

void GraphicsPipeline::SetPipelineMultisampleCreateInfo(vk::PipelineMultisampleStateCreateInfo& info){

    info.setRasterizationSamples(vk::SampleCountFlagBits::e1);
    info.setSampleShadingEnable(vk::False);
}

void GraphicsPipeline::SetPipelineColorBlendCreateInfo(vk::PipelineColorBlendStateCreateInfo& info){

    vk::PipelineColorBlendAttachmentState attachment{};

    attachment.setBlendEnable(vk::False);
    attachment.setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha);
    attachment.setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha);
    attachment.setColorBlendOp(vk::BlendOp::eAdd);
    attachment.setSrcAlphaBlendFactor(vk::BlendFactor::eOne);
    attachment.setDstColorBlendFactor(vk::BlendFactor::eZero);
    attachment.setAlphaBlendOp(vk::BlendOp::eAdd);
    attachment.setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

    info.setLogicOpEnable(false);
    info.setAttachments(attachment);
}

void GraphicsPipeline::SetPipelineLayoutCreateInfo(vk::PipelineLayoutCreateInfo& info){

    info.setSetLayoutCount(0);
    info.setPushConstantRangeCount(0);
    mPipelineLayout = std::make_unique<vk::raii::PipelineLayout>(*mLogicalDevice.GetLogicalDevice(), info);
}

void GraphicsPipeline::SetPiplineRenderCreateInfo(vk::PipelineRenderingCreateInfo& info, const std::unique_ptr<CustomSC>& swapchain){

    info.setColorAttachmentCount(1);
    info.setPColorAttachmentFormats(&swapchain->GetFormat().format);
}

void GraphicsPipeline::SetGraphicsPipilineCreateInfo(vk::GraphicsPipelineCreateInfo& info, vk::PipelineShaderStageCreateInfo shaderStages[],
                                   vk::PipelineVertexInputStateCreateInfo& vertexInput, vk::PipelineInputAssemblyStateCreateInfo& inputAssemblyInfo,
                                   vk::PipelineViewportStateCreateInfo& viewportInfo, vk::PipelineRasterizationStateCreateInfo& rasterInfo,
                                   vk::PipelineMultisampleStateCreateInfo& multisampleInfo, vk::PipelineColorBlendStateCreateInfo& colourBlendInfo,
                                   vk::PipelineDynamicStateCreateInfo& dynamicInfo, vk::PipelineRenderingCreateInfo& renderInfo)
{

    info.setPNext(renderInfo);
    info.setStageCount(2);
    info.setPStages(shaderStages);
    info.setPVertexInputState(&vertexInput);
    info.setPInputAssemblyState(&inputAssemblyInfo);
    info.setPViewportState(&viewportInfo);
    info.setPRasterizationState(&rasterInfo);
    info.setPMultisampleState(&multisampleInfo);
    info.setPColorBlendState(&colourBlendInfo);
    info.setPDynamicState(&dynamicInfo);
    info.setLayout(*mPipelineLayout);
    info.setRenderPass(nullptr);
}
