#include "Renderer.h"


Renderer::Renderer(const std::unique_ptr<CustomSC>& swapchain){
    mViewport = vk::Viewport{ 0.0f, 0.0f, (float)swapchain->GetExtent().width, (float)swapchain->GetExtent().height, 0.0f, 1.0f};
    mScissor = vk::Rect2D {vk::Offset2D{0,0}, swapchain->GetExtent()};
}
