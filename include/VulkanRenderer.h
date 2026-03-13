#ifndef RENDERER_H
#define RENDERER_H

#include "VulkanLogicalDevice.h"
#include "VKCommandBuffer.h"
#include "VulkanSwapChain.h"

class VulkanRenderer{

public:

    explicit VulkanRenderer(const std::unique_ptr<VulkanSwapChain>& swapchain);
    void Cleanup();

    bool CreateSyncObjects(const std::unique_ptr<VulkanLogicalDevice>& lDevice, const std::unique_ptr<VulkanSwapChain>& swapchain);

    void DrawFrame(const std::unique_ptr<VulkanLogicalDevice>& lDevice, const std::unique_ptr<VulkanSwapChain>& swapchain, const std::unique_ptr<VKCommandBuffer>& cmdBuffer,
                   const std::unique_ptr<GraphicsPipeline>& pipeline, GLFWwindow* window, const std::unique_ptr<VulkanSurface>& surface,const std::unique_ptr<VulkanPhysicalDevice>& pDevice,
                   const std::unique_ptr<VKVertexBuffer>& vertexBuffer, const std::unique_ptr<IndexBuffer>& indexBuffer, const std::vector<uint16_t> &indices);

    [[nodiscard]] vk::Viewport GetViewport() const {return mViewport; }
    [[nodiscard]] vk::Rect2D GetScissor() const {return mScissor; }

    void ResizeFrameBuffer() { mFrameBufferResized = true; }

private:

    bool SetupSemaphores(const std::unique_ptr<VulkanLogicalDevice>& lDevice, const std::unique_ptr<VulkanSwapChain>& swapchain);
    bool SetupFences(const std::unique_ptr<VulkanLogicalDevice>& lDevice, const std::unique_ptr<VulkanSwapChain>& swapchain);
    bool GetNextImage(const std::unique_ptr<VulkanSwapChain>& swapchain, GLFWwindow* window,const std::unique_ptr<VulkanSurface>& surface,
                      const std::unique_ptr<VulkanPhysicalDevice>& pDevice, const std::unique_ptr<VulkanLogicalDevice>& lDevice);

    //void RecordCommandBuffer(const std::unique_ptr<CustomLD>& lDevice);
    vk::SubmitInfo SubmitCommandBuffer(const std::unique_ptr<VKCommandBuffer>& cmdBuffer);
    vk::PresentInfoKHR PresentToSwapchain(const std::unique_ptr<VulkanSwapChain>& swapchain);

    std::vector<vk::raii::Semaphore> mPresentCompleteSemaphore;
    std::vector<vk::raii::Semaphore> mRenderFinishedSemaphores;
    std::vector<vk::raii::Fence> mInFlightFences;

    vk::Viewport mViewport;
    vk::Rect2D mScissor;

    uint32_t mCurrentFrame = 0;
    uint32_t mImageIndex = 0;

    bool mFrameBufferResized = false;
    bool mNeedToRecreateSwapchain = false;

};

#endif // RENDERER_H
