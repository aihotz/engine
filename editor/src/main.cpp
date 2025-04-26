#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <SDL3/SDL.h>
#include <stdio.h>

int main(int, char**)
{
    // sdl init
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        return -1;
    }

    // create window with SDL_Renderer graphics context
    Uint32      window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
    SDL_Window* window       = SDL_CreateWindow("Dear ImGui SDL3+SDL_Renderer example", 1280, 720, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (renderer == nullptr)
    {
        SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
        return -1;
    }

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // enable Docking

    // setup Dear ImGui style
    ImGui::StyleColorsDark();

    // setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    // our state
    bool   show_demo_window    = true;
    bool   show_another_window = false;
    ImVec4 clear_color         = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // main loop
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        // start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // show the big demo window
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // rendering
        ImGui::Render();
        //SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    // cleanup
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


//#include "core.hpp"
//#include "logger.hpp"
//#include <Windows.h>
//#include "input/input_manager.hpp"
//
//using GetGameFn = iGameProject * (*)();
//
//
//int main(void)
//{
//    shared::Log("We are in the editor now!");
//
//    HMODULE gameDll = LoadLibraryA("game_lib.dll");
//    if (!gameDll) {
//        shared::Log("Failed to load game_lib.dll");
//        return 1;
//    }
//
//    GetGameFn createGame = (GetGameFn)GetProcAddress(gameDll, "GetGameProject");
//    if (!createGame) {
//        shared::Log("Failed to find GetGameProject in DLL");
//        FreeLibrary(gameDll);
//        return 1;
//    }
//
//    iGameProject* game = createGame();
//    if (game) {
//        shared::Log("Game instance created successfully!");
//        
//        Engine engine;
//        engine.SetGame(game);
//        engine.Run();
//    }
//
//    // Optional cleanup
//    FreeLibrary(gameDll);
//    return 0;
//}