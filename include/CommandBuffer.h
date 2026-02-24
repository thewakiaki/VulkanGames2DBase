#ifndef COMMAND_BUFFER_H
#define COMMAND_BUFFER_H

#include "CustomPD.h"
#include "CustomLD.h"
#include "CustomSC.h"
#include "GraphicsPipeline.h"
#include <memory>

class CmdBuffer{

public:
    CmdBuffer(const std::unique_ptr<GraphicsPipeline>& pipeline) : mGraphicPipeline(pipeline){};

    bool CreateCommandPool(const std::unique_ptr<CustomPD>& pDevice, const std::unique_ptr<CustomLD>& lDevice);
    bool CreateCommandBuffer(const std::unique_ptr<CustomLD>& lDevice);

    void BeginRender(const std::unique_ptr<CustomSC>& swapchain, uint32_t imageIndex);
    void BindToGraphicsPipeline();
    void EndRender(const std::unique_ptr<CustomSC>& swapchain, uint32_t imageIndex);
    void RecordCommandBuffer(const std::unique_ptr<CustomSC>& swapchain, uint32_t imageIndex, const std::unique_ptr<GraphicsPipeline>& pipeline);

    inline const std::unique_ptr<vk::raii::CommandBuffer>& GetCommandBuffer() const { return mCommandBuffer; }

private:
    void TransitionImageLayout(uint32_t imageIndex, const std::unique_ptr<CustomSC>& swapchain, vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
                               vk::AccessFlags2 srcAccessMask, vk::AccessFlags2 dstAccessMask, vk::PipelineStageFlags2 srcStageMask,
                               vk::PipelineStageFlags2 dstStageMask);

    void SetViewportScissor(const std::unique_ptr<CustomSC>& swapchain);

    std::unique_ptr<vk::raii::CommandPool> mCommandPool;
    std::unique_ptr<vk::raii::CommandBuffer> mCommandBuffer;

    const std::unique_ptr<GraphicsPipeline>& mGraphicPipeline;

};

#endif // COMMAND_BUFFER_H
