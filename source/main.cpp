#include "../include/GameApp.h"

int main()
{
    try {

        GameApp game_app;

        game_app.Run();
        std::cout << "Finished\n";
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
