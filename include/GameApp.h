#ifndef GAME_APP_H
#define GAME_APP_H



#include "GameWindow.h"
#include "VkSetup.h"
#include "CustomSurface.h"
#include "CustomPD.h"
#include "CustomLD.h"
#include "CustomSC.h"
#include "GraphicsPipeline.h"
#include "CommandBuffer.h"
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class GameApp
{
public:

    GameApp();
    void Cleanup();

    bool Run();

private:


    bool GameStart();
    bool GamePlaying();

    void GameEnd() const;
    void InitEngineComponents();

    bool mApp_running = false;
    bool mPlaying = false;
    bool mGame_Paused = false;

    std::vector<CustomVKStructs::Vertex> mVertices;
    std::vector<uint16_t> mIndices;

    std::unique_ptr<VkSetup> mVkInstance;
    std::unique_ptr<CustomPD> mPhysicalDevice;
    std::unique_ptr<CustomLD> mLogicalDevice;
    std::unique_ptr<GameWindow> mGameWindow;
    std::unique_ptr<CustomSurface> mCustomSurface;
    std::unique_ptr<CustomSC> mSwapChain;
    std::unique_ptr<GraphicsPipeline> mGraphicsPipeline;
    std::unique_ptr<CmdBuffer> mCommandBuffer;
    std::unique_ptr<Renderer> mRenderer;
    std::unique_ptr<VertexBuffer> mVertexBuffer;
    std::unique_ptr<IndexBuffer> mIndexBuffer;

};

#endif // GAME_APP_H
