//
// Created by wakiaki on 1/12/26.
//

#ifndef VULKANGAMES2DBASE_GAMEWINDOW_H
#define VULKANGAMES2DBASE_GAMEWINDOW_H


#include "vulkan/vulkan.hpp"
class VulkanRenderer;

class GameWindow {
public:
    GameWindow();
    ~GameWindow();

    static constexpr uint32_t initial_window_width = 800;
    static constexpr uint32_t initial_window_height = 600;

    [[nodiscard]] GLFWwindow* GetWindow() const { return mGame_window;}

    bool InitGameWindow();

    void Cleanup();

    static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height);

    void SetRenderer(VulkanRenderer* renderer) { mRenderer = renderer; }


private:
    [[nodiscard]] vk::Extent2D GetFrameBufferSize() const;

    GLFWwindow* mGame_window;

    VulkanRenderer* mRenderer = nullptr;

    const char* mGame_Title = "Game Title";


};


#endif //VULKANGAMES2DBASE_GAMEWINDOW_H
