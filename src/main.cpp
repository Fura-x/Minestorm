
#include <stdlib.h> 
#include <crtdbg.h>

#include <string>
#include <time.h>

#include "raylib.h"

#include "defines.h"

#include "game.h"
#include "float2.h"

// high perf configuration
#if 1
extern "C"
{
    __declspec(dllexport) int NvOptimusEnablement = 1;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

int main(int argc, char* argv[])
{
    //Leak test
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");

    // Initialize audio device
    InitAudioDevice();      

    Music music = LoadMusicStream("media/sounds/MiniMelodies-Minestorm_OST-01_Minestorm_OST-Title_Theme.mp3");
    PlayMusicStream(music);
    SetMusicVolume(music, 1);

    //init random
    srand((unsigned int)time(0));

    //create the main game
    Game game;

    SetTargetFPS(240);
    SetConfigFlags(FLAG_VSYNC_HINT);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // UPDATE
        game.update();


        // DRAW
        BeginDrawing();

        ClearBackground(BLACK);

        game.draw();

        EndDrawing();
    }

    CloseAudioDevice();

    CloseWindow();        // Close window and OpenGL context

    return 0;
}