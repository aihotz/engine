#include "core.hpp"
#include "logger.hpp"
#include <Windows.h>
#include "input/input_manager.hpp"

using GetGameFn = iGameProject * (*)();


int main(void)
{
    shared::Log("We are in the editor now!");

    HMODULE gameDll = LoadLibraryA("game_lib.dll");
    if (!gameDll) {
        shared::Log("Failed to load game_lib.dll");
        return 1;
    }

    GetGameFn createGame = (GetGameFn)GetProcAddress(gameDll, "GetGameProject");
    if (!createGame) {
        shared::Log("Failed to find GetGameProject in DLL");
        FreeLibrary(gameDll);
        return 1;
    }

    iGameProject* game = createGame();
    if (game) {
        shared::Log("Game instance created successfully!");
        
        Engine engine;
        engine.SetGame(game);
        engine.Run();
    }

    // Optional cleanup
    FreeLibrary(gameDll);
    return 0;
}