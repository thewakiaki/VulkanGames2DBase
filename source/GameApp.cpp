#include "../include/GameApp.h"
#include <iostream>
#include <GLFW/glfw3.h>

#include <GameWindow.h>
#include <VkSetup.h>
#include <CustomSurface.h>
#include <CustomPD.h>
#include <CustomLD.h>
#include <CustomSC.h>
#include <CustomIV.h>
#include <GraphicsPipeline.h>
#include <CommandBuffer.h>
#include <Renderer.h>


GameApp::GameApp() {}


bool GameApp::Run()
{
    std::cout << "Game Starting Up\n";

    if(!glfwInit()){
        std::cerr << "Failed to init glfw\n";
        return false;
    }

    InitEngineComponents();

    if(!mGameWindow->InitGameWindow()) { return false; }

    if (!mVkInstance->InitVulkan()) { return false; }

    if(!mCustomSurface->CreateSurface(mVkInstance->GetInstance(), mGameWindow->GetWindow())) { return false; }

    if(!mPhysicalDevice->SetUpPhysicalDevice(mVkInstance->GetInstance(), mCustomSurface)) { return false; }

    if(!mCustomSurface->SetupSurfaceVariables(mGameWindow->GetWindow())) { return false; }

    if(!mLogicalDevice->CreateLogicalDevice(*mPhysicalDevice)) { return false; }

    if(!mSwapChain->CreateSwapchain(mGameWindow->GetWindow(), mCustomSurface, mPhysicalDevice, mLogicalDevice)) { return false; }

    if(!mSwapChain->CreateImageViews(mLogicalDevice)) { return false; }

    if(!mImageView->CreateImageViews(mSwapChain, mLogicalDevice)) { return false; }

    if(!mGraphicsPipeline->SetupShaders()) { return false; }

    if(!mGraphicsPipeline->CreatePipeline(mSwapChain)) { return false; }

    if(!mCommandPool->CreateCommandPool(mPhysicalDevice, mLogicalDevice)) { return false; }

    if(!mCommandPool->CreateCommandBuffer(mLogicalDevice)) { return false; }

    if(!mRenderer->CreateSyncObjects(mLogicalDevice)) { return false; }

    if (!GameStart()) { return false; }

    GamePlaying();

    return true;
}

bool GameApp::GameStart()
{
    if (mGameWindow->InitGameWindow())
    {
        mPlaying = true;
        return true;
    }

    std::cerr << "Game Failed to Start\n";

    return false;
}

bool GameApp::GamePlaying()
{

    while (!glfwWindowShouldClose(mGameWindow->GetWindow()))
    {
        glfwPollEvents();
        mRenderer->DrawFrame(mLogicalDevice, mSwapChain, mCommandPool, mGraphicsPipeline);

        if (glfwGetKey(mGameWindow->GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                  glfwSetWindowShouldClose(mGameWindow->GetWindow(), true);
              }

        mLogicalDevice->GetLogicalDevice()->waitIdle();
    }

    GameEnd();

    return true;
}

void GameApp::GameEnd()
{
    glfwTerminate();
    std::cout << "Game Finished\n";
}

void GameApp::InitEngineComponents(){
    mGameWindow = std::make_unique<GameWindow>();
    mVkInstance = std::make_unique<VkSetup>();
    mCustomSurface = std::make_unique<CustomSurface>();
    mPhysicalDevice = std::make_unique<CustomPD>();
    mLogicalDevice = std::make_unique<CustomLD>();
    mSwapChain = std::make_unique<CustomSC>();
    mImageView = std::make_unique<CustomIV>();
    mGraphicsPipeline = std::make_unique<GraphicsPipeline>(*mLogicalDevice);
    mCommandPool = std::make_unique<CmdBuffer>(mGraphicsPipeline);
    mRenderer = std::make_unique<Renderer>(mSwapChain);
}
