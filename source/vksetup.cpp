#include "../include/vksetup.h"

vkSetup::vkSetup() {}


void vkSetup::Run()
{
    vk::raii::Context context;

    constexpr vk::ApplicationInfo game_info
    {
        vk::StructureType::eApplicationInfo,
        nullptr,
        "Game Info",
        VK_MAKE_VERSION(1, 0, 0),
        "No Engine",
        VK_MAKE_VERSION(1, 0, 0),
        vk::ApiVersion14
    };

    vk::InstanceCreateInfo create_info;
    create_info.setPApplicationInfo(&game_info);

    vk::raii::Instance vulkan_instance = vk::raii::Instance(context, create_info);

    GameStart();
    GamePlaying();
    GameEnd();

}

void vkSetup::GameStart()
{
    std::cout << "Game Starting\n";
}

void vkSetup::GamePlaying()
{
    std::cout << "Now Playing\n";
}

void vkSetup::GameEnd()
{
    std::cout << "Game Finished\n";
}






