#include "../include/vksetup.h"

int main()
{
    vkSetup vk;

    try {
        vk.Run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
