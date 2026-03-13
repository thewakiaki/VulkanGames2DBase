#include "../include/GameApp.h"
#include <GLFW/glfw3.h>
#include <GameWindow.h>
#include <VulkanInstance.h>
#include <VulkanSurface.h>
#include <VulkanPhysicalDevice.h>
#include <VulkanLogicalDevice.h>
#include <VulkanSwapChain.h>
#include <GraphicsPipeline.h>
#include <VKCommandBuffer.h>
#include <VulkanRenderer.h>


GameApp::GameApp() {}


bool GameApp::Run()
{
    std::cout << "Game Starting Up\n";

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

    mGraphicsPipeline = std::make_unique<GraphicsPipeline>(*mLogicalDevice);

    if(!mSwapChain->CreateSwapchain(mGameWindow->GetWindow(), mCustomSurface, mPhysicalDevice, mLogicalDevice)) { return false; }

    if(!mSwapChain->CreateImageViews(mLogicalDevice)) { return false; }

    mRenderer = std::make_unique<VulkanRenderer>(mSwapChain);

    if(!mGraphicsPipeline->SetupShaders()) { return false; }

    if(!mGraphicsPipeline->CreatePipeline(mSwapChain)) { return false; }

    if(!mCommandBuffer->CreateCommandPool(mPhysicalDevice, mLogicalDevice)) { return false; }

    if(!mCommandBuffer->CreateCommandBuffers(mLogicalDevice)) { return false; }

    if(!mRenderer->CreateSyncObjects(mLogicalDevice, mSwapChain)) { return false; }

    if (!mVertexBuffer->SetupBuffers(mLogicalDevice, mPhysicalDevice, mVertices)) { return false; }

    mVertexBuffer->CopyStagingData(mCommandBuffer, mLogicalDevice);

    //if (!mIndexBuffer->SetupBuffers(mLogicalDevice, mPhysicalDevice, mIndices)) { return false; }

    //mIndexBuffer->CopyStagingData(mCommandBuffer, mLogicalDevice);

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
        std::cout << "playing\n";

        if (glfwGetKey(mGameWindow->GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                  glfwSetWindowShouldClose(mGameWindow->GetWindow(), true);
              }
    }

    GameEnd();

    return true;
}

void GameApp::GameEnd() {
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
    mVkInstance = std::make_unique<VulkanInstance>();
    mCustomSurface = std::make_unique<VulkanSurface>();
    mPhysicalDevice = std::make_unique<VulkanPhysicalDevice>();
    mLogicalDevice = std::make_unique<VulkanLogicalDevice>();
    mSwapChain = std::make_unique<VulkanSwapChain>();
    mCommandBuffer = std::make_unique<VKCommandBuffer>();
    mVertexBuffer = std::make_unique<VKVertexBuffer>();
    mIndexBuffer = std::make_unique<IndexBuffer>();

    //just test thing to draw
    mVertices = {
        {{0.0f, -0.5f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
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
