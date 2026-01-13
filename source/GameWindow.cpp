//
// Created by wakiaki on 1/12/26.
//

#include "../include/GameWindow.h"

GameWindow::GameWindow()
{
    mGame_window = nullptr;
}

GameWindow::~GameWindow()
{
    if (mGame_window)
    {
        CleanupGameWindow();
    }

}

bool GameWindow::InitGameWindow()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glfwInit();

    mGame_window = glfwCreateWindow(initial_window_width, initial_window_height, mGame_Title, nullptr, nullptr);

    if (mGame_window) { return true; }

    return false;
}

void GameWindow::CleanupGameWindow() const {
    glfwDestroyWindow(mGame_window);
    glfwTerminate();
}
