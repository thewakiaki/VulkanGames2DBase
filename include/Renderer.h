#ifndef RENDERER_H
#define RENDERER_H

#include "CustomLD.h"
#include "CommandBuffer.h"
#include "CustomSC.h"
#include "GraphicsPipeline.h"
#include "vulkan/vulkan.hpp"
#include <vector>

class Renderer{

public:

    explicit Renderer(const std::unique_ptr<CustomSC>& swapchain);
    void Cleanup();

    bool CreateSyncObjects(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomSC>& swapchain);

    bool DrawFrame(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomSC>& swapchain, const std::unique_ptr<CmdBuffer>& cmdBuffer,
                   const std::unique_ptr<GraphicsPipeline>& pipeline, GLFWwindow* window, const std::unique_ptr<CustomSurface>& surface,const std::unique_ptr<CustomPD>& pDevice);

    [[nodiscard]] vk::Viewport GetViewport() const {return mViewport; }
    [[nodiscard]] vk::Rect2D GetScissor() const {return mScissor; }

    inline void ResizeFrameBuffer() { mFrameBufferResized = true; }

private:

    bool SetupSemaphores(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomSC>& swapchain);
    bool SetupFences(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomSC>& swapchain);
    bool GetNextImage(const std::unique_ptr<CustomSC>& swapchain, GLFWwindow* window,const std::unique_ptr<CustomSurface>& surface,
                      const std::unique_ptr<CustomPD>& pDevice, const std::unique_ptr<CustomLD>& lDevice);

    //void RecordCommandBuffer(const std::unique_ptr<CustomLD>& lDevice);
    vk::SubmitInfo SubmitCommandBuffer(const std::unique_ptr<CmdBuffer>& cmdBuffer);
    vk::PresentInfoKHR PresentToSwapchain(const std::unique_ptr<CustomSC>& swapchain);

    std::vector<vk::raii::Semaphore> mImageAvailableSemaphores;
    std::vector<vk::raii::Semaphore> mRenderFinishedSemaphores;
    std::vector<vk::raii::Fence> mDrawFences;

    vk::Viewport mViewport;
    vk::Rect2D mScissor;

    uint32_t mCurrentFrame = 0;
    uint32_t mImageIndex = 0;

    bool mFrameBufferResized = false;
};

#endif // RENDERER_H
