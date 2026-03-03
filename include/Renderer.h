#ifndef RENDERER_H
#define RENDERER_H

#include "CustomLD.h"
#include "CommandBuffer.h"
#include "CustomSC.h"
#include "GraphicsPipeline.h"




class Renderer{

public:
    Renderer(const std::unique_ptr<CustomSC>& swapchain);

private:

    static constexpr size_t MAX_FRAMES_IN_FLIGHT = 2;

    vk::Viewport mViewport;
    vk::Rect2D mScissor;
};

#endif // RENDERER_H
