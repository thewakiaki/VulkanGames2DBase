#ifndef GRAPHICS_PIPELINE_H
#define GRAPHICS_PIPELINE_H

#include "CustomSC.h"
#include "CustomSM.h"
#include "CustomLD.h"
#include "vulkan/vulkan.hpp"
#include <memory>
#include <vulkan/vulkan_raii.hpp>


class GraphicsPipeline{
public:
    GraphicsPipeline(const CustomLD& lDevice) : mLogicalDevice(lDevice){}

    bool SetupShaders();
    bool CreatePipeline(const std::unique_ptr<CustomSC>& swapchain);

    inline const std::unique_ptr<vk::raii::Pipeline>& GetGraphicsPipeline() const { return mGraphicsPipeline; }


private:
    void SetPipelineVertShaderCreateInfo(vk::PipelineShaderStageCreateInfo& info, std::unique_ptr<CustomSM>& shader);
    void SetPipelineFragShaderCreateInfo(vk::PipelineShaderStageCreateInfo& info, std::unique_ptr<CustomSM>& shader);
    void SetPipelineDynamicCreateInfo(vk::PipelineDynamicStateCreateInfo& info);
    void SetPipelineVertInputCreateInfo(vk::PipelineVertexInputStateCreateInfo& info);
    void SetPipelineInputAssemblyCreateInfo(vk::PipelineInputAssemblyStateCreateInfo& info);
    void SetPipelineRasterCreateInfo(vk::PipelineRasterizationStateCreateInfo& info);
    void SetPipelineMultisampleCreateInfo(vk::PipelineMultisampleStateCreateInfo& info);
    void SetPipelineColorBlendCreateInfo(vk::PipelineColorBlendStateCreateInfo& info);
    void SetPipelineLayoutCreateInfo(vk::PipelineLayoutCreateInfo& info);
    void SetPiplineRenderCreateInfo(vk::PipelineRenderingCreateInfo& info, const std::unique_ptr<CustomSC>& swapchain);

    void SetGraphicsPipilineCreateInfo(vk::GraphicsPipelineCreateInfo& info, vk::PipelineShaderStageCreateInfo shaderStages[],
                                       vk::PipelineVertexInputStateCreateInfo& vertexInput, vk::PipelineInputAssemblyStateCreateInfo& inputAssemblyInfo,
                                       vk::PipelineViewportStateCreateInfo& viewportInfo, vk::PipelineRasterizationStateCreateInfo& rasterInfo,
                                       vk::PipelineMultisampleStateCreateInfo& multisampleInfo, vk::PipelineColorBlendStateCreateInfo& colourBlendInfo,
                                       vk::PipelineDynamicStateCreateInfo& dynamicInfo, vk::PipelineRenderingCreateInfo& renderInfo);

    std::unique_ptr<vk::raii::Pipeline> mGraphicsPipeline;
    std::unique_ptr<vk::raii::PipelineLayout> mPipelineLayout;

    const CustomLD& mLogicalDevice;

    std::unique_ptr<CustomSM> mVertShader;
    std::unique_ptr<CustomSM> mFragShader;

};

#endif // GRAPHICS_PIPELINE_H
