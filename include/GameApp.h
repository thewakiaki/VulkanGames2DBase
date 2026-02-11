#ifndef VKSETUP_H
#define VKSETUP_H

#include "CustomSurface.h"
#include <memory>
#include <vulkan/vulkan_raii.hpp>
#define GLFW_INCLUDE_VULKAN

#include "GameWindow.h"
#include "VkSetup.h"
#include "CustomPD.h"
#include "CustomLD.h"

class GameApp
{
public:
    GameApp();

    bool Run();

private:


    bool GameStart();
    bool GamePlaying();

    void GameEnd();
    void InitEngineComponents();

    bool mApp_running = false;
    bool mPlaying = false;
    bool mGame_Paused = false;

    std::unique_ptr<VkSetup> mVkInstance;
    std::unique_ptr<CustomPD> mPhysicalDevice;
    std::unique_ptr<CustomLD> mLogicalDevice;
    std::unique_ptr<GameWindow> mGameWindow;
    std::unique_ptr<CustomSurface> mCustomSurface;

};

#endif // VKSETUP_H
