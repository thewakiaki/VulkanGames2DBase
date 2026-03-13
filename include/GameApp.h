#ifndef GAME_APP_H
#define GAME_APP_H



#include "GameWindow.h"
#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanLogicalDevice.h"
#include "VulkanSwapChain.h"
#include "GraphicsPipeline.h"
#include "VKCommandBuffer.h"
#include "VulkanRenderer.h"
#include "IndexBuffer.h"
#include "VKVertexBuffer.h"

class GameApp
{
public:

    GameApp();
    void Cleanup();

    bool Run();

private:


    bool GameStart();
    bool GamePlaying();

    void GameEnd();
    void InitEngineComponents();

    bool mApp_running = false;
    bool mPlaying = false;
    bool mGame_Paused = false;

    std::vector<CustomVKStructs::Vertex> mVertices;
    std::vector<uint16_t> mIndices;

    std::unique_ptr<VulkanInstance> mVkInstance;
    std::unique_ptr<VulkanPhysicalDevice> mPhysicalDevice;
    std::unique_ptr<VulkanLogicalDevice> mLogicalDevice;
    std::unique_ptr<GameWindow> mGameWindow;
    std::unique_ptr<VulkanSurface> mCustomSurface;
    std::unique_ptr<VulkanSwapChain> mSwapChain;
    std::unique_ptr<GraphicsPipeline> mGraphicsPipeline;
    std::unique_ptr<VKCommandBuffer> mCommandBuffer;
    std::unique_ptr<VulkanRenderer> mRenderer;
    std::unique_ptr<VKVertexBuffer> mVertexBuffer;
    std::unique_ptr<IndexBuffer> mIndexBuffer;

};

#endif // GAME_APP_H
