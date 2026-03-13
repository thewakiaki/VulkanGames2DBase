#include "VulkanRenderer.h"
#include "CustomVkStructs.h"

VulkanRenderer::VulkanRenderer(const std::unique_ptr<VulkanSwapChain>& swapchain){
    mViewport = vk::Viewport{ 0.0f, 0.0f, static_cast<float>(swapchain->GetExtent().width), static_cast<float>(swapchain->GetExtent().height), 0.0f, 1.0f};
    mScissor = vk::Rect2D {vk::Offset2D{0,0}, swapchain->GetExtent()};
}

void VulkanRenderer::DrawFrame(const std::unique_ptr<VulkanLogicalDevice>& lDevice, const std::unique_ptr<VulkanSwapChain>& swapchain, const std::unique_ptr<VKCommandBuffer>& cmdBuffer,
               const std::unique_ptr<GraphicsPipeline>& pipeline, GLFWwindow* window, const std::unique_ptr<VulkanSurface>& surface,const std::unique_ptr<VulkanPhysicalDevice>& pDevice,
               const std::unique_ptr<VKVertexBuffer>& vertexBuffer, const std::unique_ptr<IndexBuffer>& indexBuffer, const std::vector<uint16_t> &indices){

    vk::Result fenceResult = lDevice->GetLogicalDevice()->waitForFences(*mInFlightFences[mCurrentFrame], vk::True, UINT64_MAX );

    if(fenceResult == vk::Result::eTimeout) {
        std::cout << "Fence timeout - skipping frame " << mCurrentFrame << std::endl;
        lDevice->GetLogicalDevice()->resetFences(*mInFlightFences[mCurrentFrame]);
        return;  // Skip frame
    }

    if(fenceResult != vk::Result::eSuccess)
    {
        //throw std::runtime_error("failed to wait for fence!");

        std::cout << "failed to wait for fence!\n";
        return;
    }

    lDevice->GetLogicalDevice()->resetFences(*mInFlightFences[mCurrentFrame]);

    mFrameBufferResized = false;

    if(!GetNextImage(swapchain, window, surface, pDevice, lDevice)) {
        if (!mNeedToRecreateSwapchain && mFrameBufferResized) {
            mNeedToRecreateSwapchain = true;
            //throw std::runtime_error("failed to acquire swap chain image!");
            std::cout << "failed to acquire swap chain image!\n";
            return;
        }
    }

    if (mNeedToRecreateSwapchain) {
        swapchain->RecreateSwapChain(window, surface, pDevice, lDevice);
        mNeedToRecreateSwapchain = false;
        return;
    }

    //cmdBuffer->GetCommandBuffers()[mCurrentFrame].reset(vk::CommandBufferResetFlagBits::eReleaseResources);
    cmdBuffer->GetCommandBuffers()[mCurrentFrame].reset();

    cmdBuffer->RecordCommandBuffer(swapchain, mImageIndex, pipeline, mCurrentFrame, vertexBuffer, indexBuffer, indices);

    lDevice->GetGraphicsQueue()->submit(SubmitCommandBuffer(cmdBuffer), *mInFlightFences[mCurrentFrame]);

    vk::Result presentResult = lDevice->GetPresentQueue()->presentKHR(PresentToSwapchain(swapchain));

    if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR || mFrameBufferResized)
    {
        mFrameBufferResized = false;
        return;
    }

    mCurrentFrame = (mCurrentFrame + 1 ) % CustomVKStructs::MAX_FRAMES_IN_FLIGHT;
}

bool VulkanRenderer::CreateSyncObjects(const std::unique_ptr<VulkanLogicalDevice>& lDevice, const std::unique_ptr<VulkanSwapChain>& swapchain){

    if(!SetupSemaphores(lDevice, swapchain)) { return false; }

    if(!SetupFences(lDevice, swapchain)) { return false; }

    return true;
}

bool VulkanRenderer::SetupSemaphores(const std::unique_ptr<VulkanLogicalDevice>& lDevice, const std::unique_ptr<VulkanSwapChain>& swapchain){
    assert(mPresentCompleteSemaphore.empty() && mRenderFinishedSemaphores.empty());

    try {

        for(size_t i = 0; i < swapchain->GetSwapchain()->getImages().size(); ++i)
        {
            mRenderFinishedSemaphores.emplace_back(*lDevice->GetLogicalDevice(), vk::SemaphoreCreateInfo());
        }

        for(size_t i = 0; i < CustomVKStructs::MAX_FRAMES_IN_FLIGHT; ++i)
        {
            mPresentCompleteSemaphore.emplace_back(*lDevice->GetLogicalDevice(), vk::SemaphoreCreateInfo());
        }

        std::cout << "Semaphores Create Successfully\n";
        return true;

    } catch (const vk::SystemError& err) {

        std::cerr << "Failed To Create Semaphores: " << err.what() << "\n";
        return false;
    }
}

bool VulkanRenderer::SetupFences(const std::unique_ptr<VulkanLogicalDevice>& lDevice, const std::unique_ptr<VulkanSwapChain>& swapchain){

    try {

        vk::FenceCreateInfo fenceInfo;
        fenceInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);

        for(size_t i = 0; i < CustomVKStructs::MAX_FRAMES_IN_FLIGHT; ++i)
        {
            mInFlightFences.emplace_back(*lDevice->GetLogicalDevice(), fenceInfo);
        }

        std::cout << "Fences Create Successfully\n";
        return true;

    } catch (const vk::SystemError& err) {

        std::cerr << "Failed to Create Fences Error: " << err.what() << "\n";
        return false;
    }
}

bool VulkanRenderer::GetNextImage(const std::unique_ptr<VulkanSwapChain>& swapchain, GLFWwindow* window,const std::unique_ptr<VulkanSurface>& surface,
                     const std::unique_ptr<VulkanPhysicalDevice>& pDevice, const std::unique_ptr<VulkanLogicalDevice>& lDevice){

    std::pair<vk::Result, uint32_t> acquireImageResult;

    std::tie(acquireImageResult.first, acquireImageResult.second) = swapchain->GetSwapchain()->acquireNextImage(UINT64_MAX, *mPresentCompleteSemaphore[mCurrentFrame], nullptr);

    if(acquireImageResult.first == vk::Result::eSuccess)
    {
        mImageIndex = acquireImageResult.second;

        return true;
    }

    if(acquireImageResult.first == vk::Result::eErrorOutOfDateKHR || acquireImageResult.first == vk::Result::eSuboptimalKHR)
    {
        mNeedToRecreateSwapchain = true;
        return false;
    }

    return false;
}



vk::SubmitInfo VulkanRenderer::SubmitCommandBuffer(const std::unique_ptr<VKCommandBuffer>& cmdBuffer){

     vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);

     vk::SubmitInfo submitInfo;
     submitInfo.setWaitSemaphoreCount(1);
     submitInfo.setPWaitSemaphores(&*mPresentCompleteSemaphore[mCurrentFrame]);
     submitInfo.setPWaitDstStageMask(&waitDestinationStageMask);
     submitInfo.setCommandBufferCount(1);
     submitInfo.setPCommandBuffers(&*cmdBuffer->GetCommandBuffers()[mCurrentFrame]);
     submitInfo.setSignalSemaphoreCount(1);
     submitInfo.setPSignalSemaphores(&*mRenderFinishedSemaphores[mImageIndex]);

     return submitInfo;
}

vk::PresentInfoKHR VulkanRenderer::PresentToSwapchain(const std::unique_ptr<VulkanSwapChain>& swapchain){
    vk::PresentInfoKHR presentInfo;

    presentInfo.setWaitSemaphoreCount(1);
    presentInfo.setPWaitSemaphores(&*mRenderFinishedSemaphores[mImageIndex]);
    presentInfo.setSwapchainCount(1);
    presentInfo.setPSwapchains(&**swapchain->GetSwapchain());
    presentInfo.setPImageIndices(&mImageIndex);

    return presentInfo;
}

void VulkanRenderer::Cleanup(){
    mPresentCompleteSemaphore.clear();
    mRenderFinishedSemaphores.clear();
    mInFlightFences.clear();
}
