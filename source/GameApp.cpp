#include "../include/GameApp.h"
#include <GLFW/glfw3.h>
#include <GameWindow.h>
#include <VkSetup.h>
#include <CustomSurface.h>
#include <CustomPD.h>
#include <CustomLD.h>
#include <CustomSC.h>
#include <GraphicsPipeline.h>
#include <CommandBuffer.h>
#include <Renderer.h>


GameApp::GameApp() {}


bool GameApp::Run()
{
    std::cout << "Game Starting Up\n";

    //comment or uncomment for x11 to test with renderdoc
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

    if(!glfwInit()){
        std::cerr << "Failed to init glfw\n";
        return false;
    }

    InitEngineComponents();

    if (!GameStart()) { return false; }

    GamePlaying();

    return true;
}

bool GameApp::GameStart()
{
    if(!mGameWindow->InitGameWindow()) { return false; }

    if (!mVkInstance->InitVulkan()) { return false; }

    if(!mCustomSurface->CreateSurface(mVkInstance->GetInstance(), mGameWindow->GetWindow())) { return false; }

    if(!mPhysicalDevice->SetUpPhysicalDevice(mVkInstance->GetInstance(), mCustomSurface)) { return false; }

    if(!mCustomSurface->SetupSurfaceVariables(mGameWindow->GetWindow())) { return false; }

    if(!mLogicalDevice->CreateLogicalDevice(*mPhysicalDevice)) { return false; }

    if(!mSwapChain->CreateSwapchain(mGameWindow->GetWindow(), mCustomSurface, mPhysicalDevice, mLogicalDevice)) { return false; }

    if(!mSwapChain->CreateImageViews(mLogicalDevice)) { return false; }

    if(!mGraphicsPipeline->SetupShaders()) { return false; }

    if(!mGraphicsPipeline->CreatePipeline(mSwapChain)) { return false; }

    if(!mCommandBuffer->CreateCommandPool(mPhysicalDevice, mLogicalDevice)) { return false; }

    if(!mCommandBuffer->CreateCommandBuffers(mLogicalDevice)) { return false; }

    if(!mRenderer->CreateSyncObjects(mLogicalDevice, mSwapChain)) { return false; }

    if (!mVertexBuffer->SetupBuffers(mLogicalDevice, mPhysicalDevice, mVertices)) { return false; }

    mVertexBuffer->CopyStagingData(mCommandBuffer, mLogicalDevice);

    if (!mIndexBuffer->SetupBuffers(mLogicalDevice, mPhysicalDevice, mIndices)) { return false; }

    mIndexBuffer->CopyStagingData(mCommandBuffer, mLogicalDevice);

    mGameWindow->SetRenderer(mRenderer.get());

    return true;
}

bool GameApp::GamePlaying()
{
    //return false;

    while (!glfwWindowShouldClose(mGameWindow->GetWindow()))
    {
        glfwPollEvents();

        mRenderer->DrawFrame(mLogicalDevice, mSwapChain, mCommandBuffer, mGraphicsPipeline, mGameWindow->GetWindow(), mCustomSurface, mPhysicalDevice, mVertexBuffer,
                             mIndexBuffer, mIndices);

        if (glfwGetKey(mGameWindow->GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                  glfwSetWindowShouldClose(mGameWindow->GetWindow(), true);
              }
    }

    GameEnd();

    return true;
}

void GameApp::GameEnd() const {
    mLogicalDevice->GetLogicalDevice()->waitIdle();

    mRenderer->Cleanup();
    mIndexBuffer->Cleanup();
    mVertexBuffer->Cleanup();
    mCommandBuffer->Cleanup();
    mGraphicsPipeline->Cleanup();
    mSwapChain->Cleanup();
    mLogicalDevice->Cleanup();
    mPhysicalDevice->Cleanup();
    mCustomSurface->Cleanup();
    mVkInstance->Cleanup();
    mGameWindow->Cleanup();


    std::cout << "Game Finished\n";
}

void GameApp::InitEngineComponents(){
    mGameWindow = std::make_unique<GameWindow>();
    mVkInstance = std::make_unique<VkSetup>();
    mCustomSurface = std::make_unique<CustomSurface>();
    mPhysicalDevice = std::make_unique<CustomPD>();
    mLogicalDevice = std::make_unique<CustomLD>();
    mSwapChain = std::make_unique<CustomSC>();
    mGraphicsPipeline = std::make_unique<GraphicsPipeline>(*mLogicalDevice);
    mCommandBuffer = std::make_unique<CmdBuffer>();
    mRenderer = std::make_unique<Renderer>(mSwapChain);
    mVertexBuffer = std::make_unique<VertexBuffer>();
    mIndexBuffer = std::make_unique<IndexBuffer>();

    //just test thing to draw
    mVertices = {
        {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f }},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    mIndices = { 0, 1, 2, 2, 3, 0};
}

void GameApp::Cleanup(){
    mGameWindow.reset();
    mVkInstance.reset();
    mCustomSurface.reset();
    mPhysicalDevice.reset();
    mLogicalDevice.reset();
    mSwapChain.reset();
    mGraphicsPipeline.reset();
    mCommandBuffer.reset();
    mRenderer.reset();
    mVertexBuffer.reset();
    mIndexBuffer.reset();
}
