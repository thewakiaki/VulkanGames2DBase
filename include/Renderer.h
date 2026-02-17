#ifndef RENDERER_H
#define RENDERER_H

#include "vulkan/vulkan.hpp"
#include "CustomSC.h"

class Renderer{

public:
    Renderer(const std::unique_ptr<CustomSC>& swapchain);

private:

    vk::Viewport mViewport;
    vk::Rect2D mScissor;

};

#endif // RENDERER_H
