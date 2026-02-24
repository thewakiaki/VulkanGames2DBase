//
// Created by wakiaki on 1/12/26.
//

#include "../include/GameWindow.h"
#include <GLFW/glfw3.h>

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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);


    mGame_window = glfwCreateWindow(initial_window_width, initial_window_height, mGame_Title, nullptr, nullptr);

    if (mGame_window) {
        glfwShowWindow(mGame_window);
        return true;
    }

    return false;
}

void GameWindow::CleanupGameWindow() const {
    glfwDestroyWindow(mGame_window);
    glfwTerminate();
}
