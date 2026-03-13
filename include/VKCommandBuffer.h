#ifndef COMMAND_BUFFER_H
#define COMMAND_BUFFER_H

#include "VulkanPhysicalDevice.h"
#include "VulkanLogicalDevice.h"
#include "VulkanSwapChain.h"
#include "VKVertexBuffer.h"
#include "GraphicsPipeline.h"
#include "IndexBuffer.h"

class VKCommandBuffer{

public:
    VKCommandBuffer() = default;
    void Cleanup();

    bool CreateCommandPool(const std::unique_ptr<VulkanPhysicalDevice>& pDevice, const std::unique_ptr<VulkanLogicalDevice>& lDevice);
    bool CreateCommandBuffer(const std::unique_ptr<VulkanLogicalDevice>& lDevice);
    bool CreateCommandBuffers(const std::unique_ptr<VulkanLogicalDevice>& lDevice);

    void BeginRender(const std::unique_ptr<VulkanSwapChain>& swapchain, uint32_t imageIndex, uint32_t frameIndex) const;
    //void BeginRenderPass(const std::unique_ptr<CustomSC>& swapchain, uint32_t imageIndex);
    void BindToGraphicsPipeline(uint32_t frameIndex, const std::unique_ptr<GraphicsPipeline>& pipeline) const;
    //void EndRender(const std::unique_ptr<CustomSC>& swapchain, uint32_t imageIndex);
    void RecordCommandBuffer(const std::unique_ptr<VulkanSwapChain>& swapchain, uint32_t imageIndex, const std::unique_ptr<GraphicsPipeline>& pipeline, uint32_t frameIndex,
                             const std::unique_ptr<VKVertexBuffer>& vertexBuffer, const std::unique_ptr<IndexBuffer>& indexBuffer, const std::vector<uint16_t> &indices);

    void CopyRenderBuffer(const  std::unique_ptr<vk::raii::Buffer>& srcBuffer, const  std::unique_ptr<vk::raii::Buffer>& dstBuffer,
                          vk::DeviceSize size, const std::unique_ptr<VulkanLogicalDevice>& lDevice);

    [[nodiscard]] const std::unique_ptr<vk::raii::CommandBuffer>& GetCommandBuffer() const { return mCommandBuffer; }
    [[nodiscard]] vk::raii::CommandBuffers& GetCommandBuffers() const { return *mCommandBuffers; }


private:
    void TransitionImageLayout(uint32_t frameIndex, uint32_t imageIndex, const std::unique_ptr<VulkanSwapChain>& swapchain, vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
                               vk::AccessFlags2 srcAccessMask, vk::AccessFlags2 dstAccessMask, vk::PipelineStageFlags2 srcStageMask,
                               vk::PipelineStageFlags2 dstStageMask) const;

    void SetViewportScissor(const std::unique_ptr<VulkanSwapChain>& swapchain, uint32_t frameIndex) const;



    std::unique_ptr<vk::raii::CommandPool> mCommandPool;

    std::unique_ptr<vk::raii::CommandBuffers> mCommandBuffers;
    std::unique_ptr<vk::raii::CommandBuffer> mCommandBuffer;
};

#endif // COMMAND_BUFFER_H
