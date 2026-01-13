#ifndef VKSETUP_H
#define VKSETUP_H

#define GLFW_INCLUDE_VULKAN

#include "GameWindow.h"
#include "vkSetup.h"

class GameApp
{
public:
    GameApp();

    bool Run(GameWindow& game_window, vkSetup& setup);

private:


    bool GameStart(GameWindow& game_window);
    bool GamePlaying(const GameWindow& game_window);

    void GameEnd();

    bool mApp_running = false;
    bool mPlaying = false;
    bool mGame_Paused = false;

};

#endif // VKSETUP_H
