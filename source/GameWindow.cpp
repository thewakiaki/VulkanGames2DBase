//
// Created by wakiaki on 1/12/26.
//

#include "../include/GameWindow.h"
#include "VulkanRenderer.h"


GameWindow::GameWindow()
{
    mGame_window = nullptr;
}

GameWindow::~GameWindow()
{
    if (mGame_window)
    {
        Cleanup();
    }
}

bool GameWindow::InitGameWindow()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);


    mGame_window = glfwCreateWindow(initial_window_width, initial_window_height, mGame_Title, nullptr, nullptr);
    glfwSetWindowUserPointer(mGame_window, this);
    glfwSetFramebufferSizeCallback(mGame_window, FrameBufferResizeCallback);

    if (mGame_window) {
        glfwShowWindow(mGame_window);
        return true;
    }

    return false;
}

vk::Extent2D GameWindow::GetFrameBufferSize() const{
    int width;
    int height;

    glfwGetFramebufferSize(mGame_window, &width, &height);
    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
}

void GameWindow::Cleanup() {

    if(mGame_window) { glfwDestroyWindow(mGame_window); }
    mGame_window = nullptr;
}

void GameWindow::FrameBufferResizeCallback(GLFWwindow *window, int width, int height){
    GameWindow* gWindow = reinterpret_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    gWindow->mRenderer->ResizeFrameBuffer();
}
