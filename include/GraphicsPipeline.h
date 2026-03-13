#ifndef GRAPHICS_PIPELINE_H
#define GRAPHICS_PIPELINE_H

#include "VulkanSwapChain.h"
#include "VulkanShaderModule.h"
#include "VulkanLogicalDevice.h"



class GraphicsPipeline{
public:
    explicit GraphicsPipeline(const VulkanLogicalDevice& lDevice) : mLogicalDevice(lDevice){}

    bool SetupShaders();
    bool CreatePipeline(const std::unique_ptr<VulkanSwapChain>& swapchain);
    void Cleanup();

   [[nodiscard]] const std::unique_ptr<vk::raii::Pipeline>& GetGraphicsPipeline() const { return mGraphicsPipeline; }


private:
    void SetPipelineVertShaderCreateInfo(vk::PipelineShaderStageCreateInfo& info, const std::unique_ptr<VulkanShaderModule>& shader);
    void SetPipelineFragShaderCreateInfo(vk::PipelineShaderStageCreateInfo& info, const std::unique_ptr<VulkanShaderModule>& shader);
    void SetPipelineDynamicCreateInfo(vk::PipelineDynamicStateCreateInfo& info);
    void SetPipelineVertInputCreateInfo(vk::PipelineVertexInputStateCreateInfo& info);
    void SetPipelineInputAssemblyCreateInfo(vk::PipelineInputAssemblyStateCreateInfo& info);
    void SetPipelineRasterCreateInfo(vk::PipelineRasterizationStateCreateInfo& info);
    void SetPipelineMultisampleCreateInfo(vk::PipelineMultisampleStateCreateInfo& info);
    void SetPipelineColorBlendCreateInfo(vk::PipelineColorBlendStateCreateInfo& info);
    void SetPipelineLayoutCreateInfo(vk::PipelineLayoutCreateInfo& info);
    void SetPipelineRenderCreateInfo(vk::PipelineRenderingCreateInfo& info, const std::unique_ptr<VulkanSwapChain>& swapchain);

    void SetGraphicsPipelineCreateInfo(vk::GraphicsPipelineCreateInfo& info, vk::PipelineShaderStageCreateInfo shaderStages[],
                                       const vk::PipelineVertexInputStateCreateInfo& vertexInput, const vk::PipelineInputAssemblyStateCreateInfo& inputAssemblyInfo,
                                       const vk::PipelineViewportStateCreateInfo& viewportInfo, const vk::PipelineRasterizationStateCreateInfo& rasterInfo,
                                       const vk::PipelineMultisampleStateCreateInfo& multisampleInfo, const vk::PipelineColorBlendStateCreateInfo& colourBlendInfo,
                                       const vk::PipelineDynamicStateCreateInfo& dynamicInfo, vk::PipelineRenderingCreateInfo& renderInfo);

    std::unique_ptr<vk::raii::Pipeline> mGraphicsPipeline;
    std::unique_ptr<vk::raii::PipelineLayout> mPipelineLayout;

    const VulkanLogicalDevice& mLogicalDevice;

    std::unique_ptr<VulkanShaderModule> mVertShader;
    std::unique_ptr<VulkanShaderModule> mFragShader;

    std::vector<vk::DynamicState> mDynamicStates;

    vk::VertexInputBindingDescription mBinding = CustomVKStructs::Vertex::getBindingDescription();;
    std::array<vk::VertexInputAttributeDescription, 2> mAttributes = CustomVKStructs::Vertex::getAttributeDescriptions();

};

#endif // GRAPHICS_PIPELINE_H
