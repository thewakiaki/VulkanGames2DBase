#include "Renderer.h"
#include "CustomVkStructs.h"
#include "vulkan/vulkan.hpp"
#include <cstddef>
#include <vulkan/vulkan_raii.hpp>

Renderer::Renderer(const std::unique_ptr<CustomSC>& swapchain){
    mViewport = vk::Viewport{ 0.0f, 0.0f, (float)swapchain->GetExtent().width, (float)swapchain->GetExtent().height, 0.0f, 1.0f};
    mScissor = vk::Rect2D {vk::Offset2D{0,0}, swapchain->GetExtent()};
}

bool Renderer::DrawFrame(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomSC>& swapchain, const std::unique_ptr<CmdBuffer>& cmdBuffer,
                         const std::unique_ptr<GraphicsPipeline>& pipeline){

    vk::Result fenceResult = lDevice->GetLogicalDevice()->waitForFences(*mDrawFences[mCurrentFrame], vk::True, UINT64_MAX);

    if(fenceResult != vk::Result::eSuccess)
    {
        throw std::runtime_error("failed to wait for fence!");
        return false;
    }

    lDevice->GetLogicalDevice()->resetFences(*mDrawFences[mCurrentFrame]);

    vk::Result presentResult;

    if(!GetNextImage(swapchain)) { return false; }

    cmdBuffer->GetCommandBuffers()[mCurrentFrame].reset();

    cmdBuffer->RecordCommandBuffer(swapchain, mImageIndex, pipeline, mCurrentFrame);

    lDevice->GetGraphicsQueue()->submit(SubmitCommandBuffer(cmdBuffer), *mDrawFences[mCurrentFrame]);

    presentResult = lDevice->GetPresentQueue()->presentKHR(PresentToSwapchain(swapchain));

    mCurrentFrame = (mCurrentFrame + 1 ) % CustomVKStructs::MAX_FRAMES_IN_FLIGHT;

    return true;
};

bool Renderer::CreateSyncObjects(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomSC>& swapchain){

    if(!SetupSemaphores(lDevice, swapchain)) { return false; }

    if(!SetupFences(lDevice, swapchain)) { return false; }

    return true;
}

bool Renderer::SetupSemaphores(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomSC>& swapchain){

    try {

        for(size_t i = 0; i < swapchain->GetSwapchain()->getImages().size(); ++i)
        {
            mRenderFinishedSemaphores.emplace_back(*lDevice->GetLogicalDevice(), vk::SemaphoreCreateInfo());
        }

        for(size_t i = 0; i < CustomVKStructs::MAX_FRAMES_IN_FLIGHT; ++i)
        {
            mImageAvailableSemaphores.emplace_back(*lDevice->GetLogicalDevice(), vk::SemaphoreCreateInfo());
        }

        std::cout << "Semaphores Create Successfully\n";
        return true;

    } catch (const vk::SystemError& err) {

        std::cerr << "Failed To Create Semaphores\n";
        return false;
    }
}

bool Renderer::SetupFences(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomSC>& swapchain){

    try {

        vk::FenceCreateInfo fenceInfo;
        fenceInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);

        for(size_t i = 0; i < CustomVKStructs::MAX_FRAMES_IN_FLIGHT; ++i)
        {
            mDrawFences.emplace_back(vk::raii::Fence(*lDevice->GetLogicalDevice(), fenceInfo));
        }

        std::cout << "Fences Create Successfully\n";
        return true;
    } catch (const vk::SystemError& err) {

        std::cerr << "Fences To Create Semaphores\n";
        return false;
    }
}

bool Renderer::GetNextImage(const std::unique_ptr<CustomSC>& swapchain){

    std::pair<vk::Result, uint32_t> acquireImageResult;

    std::tie(acquireImageResult.first, acquireImageResult.second) = swapchain->GetSwapchain()->acquireNextImage(UINT64_MAX, *mImageAvailableSemaphores[mCurrentFrame], nullptr);

    if(acquireImageResult.first != vk::Result::eSuccess)
    {
        std::cerr << "Failed to Acquire Image\n";
        return false;
    }

    mImageIndex = acquireImageResult.second;

    return true;
}

vk::SubmitInfo Renderer::SubmitCommandBuffer(const std::unique_ptr<CmdBuffer>& cmdBuffer){

     vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);

     vk::SubmitInfo submitInfo;
     submitInfo.setWaitSemaphoreCount(1);
     submitInfo.setPWaitSemaphores(&*mImageAvailableSemaphores[mCurrentFrame]);
     submitInfo.setPWaitDstStageMask(&waitDestinationStageMask);
     submitInfo.setCommandBufferCount(1);
     submitInfo.setPCommandBuffers(&*cmdBuffer->GetCommandBuffers()[mCurrentFrame]);
     submitInfo.setSignalSemaphoreCount(1);
     submitInfo.setPSignalSemaphores(&*mRenderFinishedSemaphores[mImageIndex]);

     return submitInfo;
}

vk::PresentInfoKHR Renderer::PresentToSwapchain(const std::unique_ptr<CustomSC>& swapchain){
    vk::PresentInfoKHR presentInfo;
    presentInfo.setWaitSemaphoreCount(1);
    presentInfo.setPWaitSemaphores(&*mRenderFinishedSemaphores[mImageIndex]);
    presentInfo.setSwapchainCount(1);
    presentInfo.setPSwapchains(&**swapchain->GetSwapchain());
    presentInfo.setPImageIndices(&mImageIndex);
    presentInfo.setPResults(nullptr);

    return presentInfo;
}

void Renderer::Cleanup(){
    mImageAvailableSemaphores.clear();
    mRenderFinishedSemaphores.clear();
    mDrawFences.clear();
}
