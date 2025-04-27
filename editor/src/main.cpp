#include <glbinding/gl/gl.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <SDL3/SDL.h>
#include <stdio.h>

#include <input/input_manager.hpp>
#include <window/window.hpp>

// Main code
int main(int, char**)
{
    engine::Window::GetInstance().Initialize();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding                = 0.0f;
        style.Colors[ ImGuiCol_WindowBg ].w = 1.0f;
    }

    SDL_Window* window     = static_cast<SDL_Window*>(engine::Window::GetInstance().GetWindowHandle());
    void*       gl_context = engine::Window::GetInstance().GetContextHandle();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 460");

    // Our state
    bool   show_demo_window    = true;
    bool   show_another_window = false;
    ImVec4 clear_color         = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    engine::InputManager::GetInstance().Initialize();

    // Main loop
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

            switch (event.type)
            {
                case SDL_EVENT_GAMEPAD_ADDED:
                case SDL_EVENT_GAMEPAD_REMOVED:
                case SDL_EVENT_MOUSE_WHEEL:
                    engine::InputManager::GetInstance().ProcessEvents(&event);
                    break;

                case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
                case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
                    engine::Window::GetInstance().ProcessEvents(&event);
                    break;
            }
        }

        if (engine::Window::GetInstance().IsMinimized())
        {
            SDL_Delay(10);
            continue;
        }

        engine::InputManager::GetInstance().Update();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // Rendering
        ImGui::Render();
        gl::glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        gl::glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        gl::glClear(gl::ClearBufferMask::GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window*   backup_current_window  = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }

        engine::Window::GetInstance().SwapBuffers();
    }

    engine::InputManager::GetInstance().Shutdown();

    // Cleanup
    // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your SDL_AppQuit() function]
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    engine::Window::GetInstance().Shutdown();

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