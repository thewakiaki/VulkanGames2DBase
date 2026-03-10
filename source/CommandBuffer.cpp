#include "CommandBuffer.h"
#include "CustomVkStructs.h"
#include "GraphicsPipeline.h"

void CmdBuffer::Cleanup(){
    mCommandBuffer.reset();
    mCommandBuffers.reset();
    mCommandPool.reset();
}

bool CmdBuffer::CreateCommandPool(const std::unique_ptr<CustomPD>& pDevice, const std::unique_ptr<CustomLD>& lDevice){

    vk::CommandPoolCreateInfo createInfo;

    createInfo.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

    for(const CustomVKStructs::VkQueueFamilies& family : pDevice->GetFamilies())
    {
        if(family.familyType == CustomVKStructs::RequiredVkFamilies::Graphics)
        {
            createInfo.setQueueFamilyIndex(family.familyIndex);
            break;
        }
    }

    try {
        mCommandPool = std::make_unique<vk::raii::CommandPool>(*lDevice->GetLogicalDevice(), createInfo);
        std::cout << "Created Command Pool\n";

        return true;

    } catch (const vk::SystemError& err) {
        std::cerr << "Failed to Create Command Pool Error: " << err.what() << "\n";

        return false;
    }
}

bool CmdBuffer::CreateCommandBuffer(const std::unique_ptr<CustomLD>& lDevice){

    vk::CommandBufferAllocateInfo allocInfo;

    allocInfo.setCommandPool(*mCommandPool);
    allocInfo.setLevel(vk::CommandBufferLevel::ePrimary);
    allocInfo.setCommandBufferCount(1);

    vk::raii::CommandBuffers buffers = vk::raii::CommandBuffers(*lDevice->GetLogicalDevice(), allocInfo);

    try {
        mCommandBuffer = std::make_unique<vk::raii::CommandBuffer>(std::move(buffers.front()));
        std::cout << "Created Command Buffer\n";

        return true;

    } catch (const vk::SystemError& err) {
        std::cerr << "Failed to Create Command Buffer Error: " << err.what() << "\n";

        return false;
    }
}

bool CmdBuffer::CreateCommandBuffers(const std::unique_ptr<CustomLD>& lDevice){
    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.setCommandPool(*mCommandPool);
    allocInfo.setLevel(vk::CommandBufferLevel::ePrimary);
    allocInfo.setCommandBufferCount(CustomVKStructs::MAX_FRAMES_IN_FLIGHT);

    try {
         mCommandBuffers = std::make_unique<vk::raii::CommandBuffers>(*lDevice->GetLogicalDevice(), allocInfo);
         std::cout << "Created Command Buffers\n";

         return true;
    } catch (const vk::SystemError& err) {

        std::cerr << "Failed to create Command Buffers Error: " << err.what() << "\n";
        return false;
    }

}

void CmdBuffer::RecordCommandBuffer(const std::unique_ptr<CustomSC>& swapchain, uint32_t imageIndex,
                                    const std::unique_ptr<GraphicsPipeline>& pipeline, uint32_t frameIndex, const std::unique_ptr<CustomVertexBuffer>& vertexBuffer) {

    GetCommandBuffers()[frameIndex].reset(vk::CommandBufferResetFlagBits::eReleaseResources);

    GetCommandBuffers()[frameIndex].begin({});

    TransitionImageLayout(frameIndex, imageIndex, swapchain, vk::ImageLayout::eUndefined, vk::ImageLayout::eColorAttachmentOptimal, {},
                          vk::AccessFlagBits2::eColorAttachmentWrite, vk::PipelineStageFlagBits2::eColorAttachmentOutput,
                              vk::PipelineStageFlagBits2::eColorAttachmentOutput);

    BeginRender(swapchain, imageIndex, frameIndex);

    SetViewportScissor(swapchain, frameIndex);

    BindToGraphicsPipeline(frameIndex, pipeline);

    GetCommandBuffers()[frameIndex].bindVertexBuffers(0, {*vertexBuffer->GetVertexBuffer()}, {0});

    GetCommandBuffers()[frameIndex].draw(3, 1, 0, 0);

    GetCommandBuffers()[frameIndex].endRendering();

    TransitionImageLayout(frameIndex, imageIndex, swapchain, vk::ImageLayout::eColorAttachmentOptimal, vk::ImageLayout::ePresentSrcKHR, vk::AccessFlagBits2::eColorAttachmentWrite,
                          {}, vk::PipelineStageFlagBits2::eColorAttachmentOutput, vk::PipelineStageFlagBits2::eBottomOfPipe);

    GetCommandBuffers()[frameIndex].end();
}

void CmdBuffer::TransitionImageLayout(uint32_t frameIndex, uint32_t imageIndex, const std::unique_ptr<CustomSC>& swapchain, vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
                                      vk::AccessFlags2 srcAccessMask, vk::AccessFlags2 dstAccessMask, vk::PipelineStageFlags2 srcStageMask,
                                      vk::PipelineStageFlags2 dstStageMask) const {

    vk::ImageSubresourceRange resourceRange;
    resourceRange.setAspectMask(vk::ImageAspectFlagBits::eColor);
    resourceRange.setBaseMipLevel(0);
    resourceRange.setLevelCount(1);
    resourceRange.setBaseArrayLayer(0);
    resourceRange.setLayerCount(1);

    vk::ImageMemoryBarrier2 barrier;
    barrier.setSrcStageMask(srcStageMask);
    barrier.setSrcAccessMask(srcAccessMask);
    barrier.setDstStageMask(dstStageMask);
    barrier.setDstAccessMask(dstAccessMask);
    barrier.setOldLayout(oldLayout);
    barrier.setNewLayout(newLayout);
    barrier.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
    barrier.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
    barrier.setImage(swapchain->GetImages()[imageIndex]);
    barrier.setSubresourceRange(resourceRange);

    vk::DependencyInfo dependencyInfo;
    dependencyInfo.setDependencyFlags({});
    dependencyInfo.setImageMemoryBarrierCount(1);
    dependencyInfo.setPImageMemoryBarriers(&barrier);

    GetCommandBuffers()[frameIndex].pipelineBarrier2(dependencyInfo);
}

void CmdBuffer::BeginRender(const std::unique_ptr<CustomSC>& swapchain, uint32_t imageIndex, uint32_t frameIndex) const {

    vk::ClearColorValue clearColor = vk::ClearColorValue(0.045f, 0.045f, 0.045f, 1.0f);

    vk::RenderingAttachmentInfo  attachmentInfo;
    attachmentInfo.setImageView(*swapchain->GetImageViews()[imageIndex]);
    attachmentInfo.setImageLayout(vk::ImageLayout::eColorAttachmentOptimal);
    attachmentInfo.setLoadOp(vk::AttachmentLoadOp::eClear);
    attachmentInfo.setStoreOp(vk::AttachmentStoreOp::eStore);
    attachmentInfo.setClearValue(clearColor);

    vk::RenderingInfo renderingInfo;
    renderingInfo.setRenderArea({vk::Offset2D{0,0}, swapchain->GetExtent()});
    renderingInfo.setLayerCount(1);
    renderingInfo.setColorAttachmentCount(1);
    renderingInfo.setPColorAttachments(&attachmentInfo);

    GetCommandBuffers()[frameIndex].beginRendering(renderingInfo);
}

void CmdBuffer::BindToGraphicsPipeline(uint32_t frameIndex, const std::unique_ptr<GraphicsPipeline>& pipeline) const {

    GetCommandBuffers()[frameIndex].bindPipeline(vk::PipelineBindPoint::eGraphics, *pipeline->GetGraphicsPipeline());
}

void CmdBuffer::SetViewportScissor(const std::unique_ptr<CustomSC>& swapchain, uint32_t frameIndex) const {

    vk::Extent2D swapExtent = swapchain->GetExtent();

    vk::Viewport viewport = vk::Viewport(0.0f, 0.0f, static_cast<float>(swapExtent.width), static_cast<float>(swapExtent.height), 0.0f, 1.0f);

    GetCommandBuffers()[frameIndex].setViewport(0, {viewport});
    GetCommandBuffers()[frameIndex].setScissor(0, vk::Rect2D{vk::Offset2D(0, 0), swapExtent});
}
