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
    Renderer(const std::unique_ptr<CustomSC>& swapchain);
    void Cleanup();

    bool CreateSyncObjects(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomSC>& swapchain);

    bool DrawFrame(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomSC>& swapchain, const std::unique_ptr<CmdBuffer>& cmdBuffer,
                   const std::unique_ptr<GraphicsPipeline>& pipeline);

    inline const vk::Viewport GetViewport() const {return mViewport; }
    inline const vk::Rect2D GetScissor() const {return mScissor; }

private:

    bool SetupSemaphores(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomSC>& swapchain);
    bool SetupFences(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomSC>& swapchain);
    bool GetNextImage(const std::unique_ptr<CustomSC>& swapchain);

    void RecordCommandBuffer(const std::unique_ptr<CustomLD>& lDevice);
    vk::SubmitInfo SubmitCommandBuffer(const std::unique_ptr<CmdBuffer>& cmdBuffer);
    vk::PresentInfoKHR PresentToSwapchain(const std::unique_ptr<CustomSC>& swapchain);

    std::vector<vk::raii::Semaphore> mImageAvailableSemaphores;
    std::vector<vk::raii::Semaphore> mRenderFinishedSemaphores;
    std::vector<vk::raii::Fence> mDrawFences;

    vk::Viewport mViewport;
    vk::Rect2D mScissor;

    uint32_t mCurrentFrame = 0;
    uint32_t mImageIndex = 0;
};

#endif // RENDERER_H
