#include "../include/GameApp.h"
#include "CustomLD.h"
#include "CustomPD.h"
#include "CustomSurface.h"
#include "GameWindow.h"
#include "VkSetup.h"

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

        if (glfwGetKey(mGameWindow->GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                  glfwSetWindowShouldClose(mGameWindow->GetWindow(), true);
              }
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

}
