#ifndef VKSETUP_H
#define VKSETUP_H


#include "CustomSurface.h"
#include "CustomSC.h"
#include "GameWindow.h"
#include "GraphicsPipeline.h"
#include "VkSetup.h"
#include "CustomPD.h"
#include "CustomLD.h"
#include "CustomIV.h"
#include "CommandBuffer.h"
#include <memory>


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
    std::unique_ptr<CustomSC> mSwapChain;
    std::unique_ptr<CustomIV> mImageView;
    std::unique_ptr<GraphicsPipeline> mGraphicsPipeline;
    std::unique_ptr<CmdBuffer> mCommandPool;

};

#endif // VKSETUP_H
