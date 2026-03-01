#include "Renderer.h"
#include "CommandBuffer.h"
#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan.h>

#include <cstdint>
#include <memory>
#include <vulkan/vulkan_hpp_macros.hpp>
#include <vulkan/vulkan_raii.hpp>

Renderer::Renderer(const std::unique_ptr<CustomSC>& swapchain){
    mViewport = vk::Viewport{ 0.0f, 0.0f, (float)swapchain->GetExtent().width, (float)swapchain->GetExtent().height, 0.0f, 1.0f};
    mScissor = vk::Rect2D {vk::Offset2D{0,0}, swapchain->GetExtent()};
}

bool Renderer::CreateSyncObjects(const  std::unique_ptr<CustomLD>& lDevice){

    if(!CreateSemaphores(lDevice)) { return false; }

    if(!CreateFences(lDevice)) { return false; }

    return true;

}

bool Renderer::CreateSemaphores(const  std::unique_ptr<CustomLD>& lDevice){
    try {

        //for(size_t semaphore = 0; semaphore < MAX_FRAMES_IN_FLIGHT; ++semaphore)
        //{
        //    mPresentCompleteSemaphores[semaphore] = std::make_unique<vk::raii::Semaphore>(*lDevice->GetLogicalDevice(), vk::SemaphoreCreateInfo(), nullptr);
        //    mRenderFinishedSemaphores[semaphore] = std::make_unique<vk::raii::Semaphore>(*lDevice->GetLogicalDevice(), vk::SemaphoreCreateInfo(), nullptr);
        //}
        //

        mPresentCompleteSemaphore = std::make_unique<vk::raii::Semaphore>(*lDevice->GetLogicalDevice(), vk::SemaphoreCreateInfo());
        mRenderFinishedSemaphore = std::make_unique<vk::raii::Semaphore>(*lDevice->GetLogicalDevice(), vk::SemaphoreCreateInfo());

        std::cout << "Semaphores Created\n";

        return true;
    } catch (const vk::SystemError& err) {
        std::cerr << "Failed to Semaphores\n";
        return false;
    }
}

bool Renderer::CreateFences(const std::unique_ptr<CustomLD>& lDevice){


    try {
        vk::FenceCreateInfo fenceInfo;
        fenceInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);

        //for(size_t fence = 0; fence < MAX_FRAMES_IN_FLIGHT; ++fence)
        //{
        //    mDrawFences[fence] = std::make_unique<vk::raii::Fence>(*lDevice->GetLogicalDevice(), fenceInfo, nullptr);
        //}
        //

        mDrawFence = std::make_unique<vk::raii::Fence>(*lDevice->GetLogicalDevice(), fenceInfo);


        std::cout << "Fences Created\n";

        return true;
    } catch (const vk::SystemError& err) {
        std::cerr << "Failed to Create Fences\n";
        return false;
    }
}

void Renderer::DrawFrame(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomSC>& swapchain, const std::unique_ptr<CmdBuffer>& cmdBuffer,
                         const std::unique_ptr<GraphicsPipeline>& pipeline){

    //mBufferIndex = mCurrentFrame % MAX_FRAMES_IN_FLIGHT;

    vk::Result fenceResult = lDevice->GetLogicalDevice()->waitForFences({*mDrawFence}, vk::True, UINT64_MAX);

    auto [result, imageIndex] = swapchain->GetSwapchain()->acquireNextImage(UINT64_MAX, **mPresentCompleteSemaphore, nullptr);

    cmdBuffer->RecordCommandBuffer(swapchain, imageIndex, pipeline);

    lDevice->GetLogicalDevice()->resetFences({*mDrawFence});

    SubmitBuffer(cmdBuffer, lDevice);

    vk::Result presentResult = Present(swapchain, lDevice, imageIndex);
}

void Renderer::SubmitBuffer(const std::unique_ptr<CmdBuffer>& cmdBuffer, const std::unique_ptr<CustomLD>& lDevice){

    vk::Semaphore waitSemaphore[] = { *mPresentCompleteSemaphore };
    vk::Semaphore signalSemaphore[] = { *mRenderFinishedSemaphore };

    vk::PipelineStageFlags stageMask[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

    vk::SubmitInfo submitInfo;
    submitInfo.setWaitSemaphoreCount(1);
    submitInfo.setPWaitSemaphores(waitSemaphore);  // Wait for image ready
    submitInfo.setPWaitDstStageMask(stageMask);
    submitInfo.setCommandBufferCount(1);
    submitInfo.setPCommandBuffers(&**cmdBuffer->GetCommandBuffer());
    submitInfo.setSignalSemaphoreCount(1);
    submitInfo.setPSignalSemaphores(signalSemaphore);  // Signal for present

    lDevice->GetGraphicsQueue()->submit(submitInfo, *mDrawFence);
}

vk::Result Renderer::Present(const std::unique_ptr<CustomSC>& swapchain, const std::unique_ptr<CustomLD>& lDevice, uint32_t imageIndex){

    vk::Semaphore renderFinsishSemaphore[1] = { *mRenderFinishedSemaphore };

    static vk::PresentInfoKHR presentInfo;
    presentInfo.setWaitSemaphoreCount(1);
    presentInfo.setPWaitSemaphores(renderFinsishSemaphore);
    presentInfo.setSwapchainCount(1);
    presentInfo.setPSwapchains(&**swapchain->GetSwapchain());
    presentInfo.setImageIndices(imageIndex);
    presentInfo.setPResults(nullptr);

    vk::Result result = lDevice->GetPresentQueue()->presentKHR(presentInfo);

    return result;
}
