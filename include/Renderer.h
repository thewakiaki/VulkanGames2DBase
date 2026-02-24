#ifndef RENDERER_H
#define RENDERER_H

#include "CustomLD.h"
#include "CommandBuffer.h"
#include "CustomSC.h"
#include "GraphicsPipeline.h"
#include "vulkan/vulkan.hpp"
#include <array>
#include <cstdint>
#include <memory>




class Renderer{

public:
    Renderer(const std::unique_ptr<CustomSC>& swapchain);

    bool CreateSyncObjects(const  std::unique_ptr<CustomLD>& lDevice);

    void DrawFrame(const std::unique_ptr<CustomLD>& lDevice, const std::unique_ptr<CustomSC>& swapchain, const std::unique_ptr<CmdBuffer>& cmdBuffer,
                   const std::unique_ptr<GraphicsPipeline>& pipeline);

private:
    bool CreateSemaphores(const std::unique_ptr<CustomLD>& lDevice);
    bool CreateFences(const std::unique_ptr<CustomLD>& lDevice);

    void SubmitBuffer(const std::unique_ptr<CmdBuffer>& cmdBuffer, const std::unique_ptr<CustomLD>& lDevice);
    void Present(const std::unique_ptr<CustomSC>& swapchain, const std::unique_ptr<CustomLD>& lDevice, uint32_t imageIndex);

    static constexpr size_t MAX_FRAMES_IN_FLIGHT = 2;

    vk::Viewport mViewport;
    vk::Rect2D mScissor;

    std::unique_ptr<vk::raii::Semaphore> mPresentCompleteSemaphore;
    std::unique_ptr<vk::raii::Semaphore> mRenderFinishedSemaphore;
    std::unique_ptr<vk::raii::Fence> mDrawFence;

    std::array<std::unique_ptr<vk::raii::Semaphore> , MAX_FRAMES_IN_FLIGHT> mPresentCompleteSemaphores = { std::make_unique<vk::raii::Semaphore>(nullptr) };
    std::array<std::unique_ptr<vk::raii::Semaphore>, MAX_FRAMES_IN_FLIGHT> mRenderFinishedSemaphores = { std::make_unique<vk::raii::Semaphore>(nullptr) };
    std::array<std::unique_ptr<vk::raii::Fence>, MAX_FRAMES_IN_FLIGHT> mDrawFences = { std::make_unique<vk::raii::Fence>(nullptr) };

    size_t mBufferIndex = 0;
    size_t mCurrentFrame = 0;
};

#endif // RENDERER_H
