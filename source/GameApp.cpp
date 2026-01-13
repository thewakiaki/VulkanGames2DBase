#include "../include/GameApp.h"
#include "GameWindow.h"

GameApp::GameApp() {}


bool GameApp::Run(GameWindow& game_window, vkSetup& setup)
{
    if (!setup.InitVulkan()) { return false; }

    if (!GameStart(game_window)) { return false; }

    GamePlaying(game_window);

    return true;
}

bool GameApp::GameStart(GameWindow& game_window)
{
    if (game_window.InitGameWindow())
    {
        mPlaying = true;
        return true;
    }

    return false;
}

bool GameApp::GamePlaying(const GameWindow& game_window)
{

    while (!glfwWindowShouldClose(game_window.GetWindow()))
    {
        glfwPollEvents();
    }

    GameEnd();

    return true;
}

void GameApp::GameEnd()
{
    std::cout << "Game Finished\n";
}






