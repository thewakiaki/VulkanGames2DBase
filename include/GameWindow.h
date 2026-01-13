//
// Created by wakiaki on 1/12/26.
//

#ifndef VULKANGAMES2DBASE_GAMEWINDOW_H
#define VULKANGAMES2DBASE_GAMEWINDOW_H



class GameWindow {
public:
    GameWindow();
    ~GameWindow();

    static constexpr uint32_t initial_window_width = 800;
    static constexpr uint32_t initial_window_height = 600;

    inline GLFWwindow* GetWindow() const { return mGame_window;}

    bool InitGameWindow();

private:
    GLFWwindow *mGame_window;

    const char* mGame_Title = "Game Title";


    void CleanupGameWindow() const;

};


#endif //VULKANGAMES2DBASE_GAMEWINDOW_H