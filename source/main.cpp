#include "../include/GameApp.h"
#include "../include/GameWindow.h"
#include "../include/VkSetup.h"

int main()
{
    try {
        VkSetup vulkan;
        GameApp game_app;
        GameWindow game_window;

        game_app.Run(game_window, vulkan);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
