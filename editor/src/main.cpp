#include <stdio.h>

#include <SDL3/SDL.h>
#include <glbinding/gl/gl.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <imgui_internal.h>

#include <engine/game_object/component.hpp>
#include <engine/game_object/game_object.hpp>
#include <engine/game_object/game_object_manager.hpp>
#include <engine/input/input_manager.hpp>
#include <engine/window/window.hpp>

void MainInitialize();
void MainDrawing();
void MainShutdown();

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

    MainInitialize();

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

        engine::GameObjectManager::GetInstance().Update();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        MainDrawing();

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

    MainShutdown();

    engine::GameObjectManager::GetInstance().Shutdown();

    engine::InputManager::GetInstance().Shutdown();

    // Cleanup
    // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your SDL_AppQuit() function]
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    engine::Window::GetInstance().Shutdown();

    return 0;
}

struct ComponentA : public engine::Component
{
    virtual void OnCreate() override
    {
        printf("ComponentA");
    }

    virtual void OnShutdown() override
    {
        printf("~ComponentA");
    }
};
struct ComponentB : public engine::Component
{
    virtual void OnCreate() override
    {
        printf("ComponentB");
    }

    virtual void OnShutdown() override
    {
        printf("~ComponentB");
    }
};
struct ComponentC : public engine::Component
{
    virtual void OnCreate() override
    {
        printf("ComponentC");
    }

    virtual void OnShutdown() override
    {
        printf("~ComponentC");
    }
};

namespace
{
    engine::GameObject* selected_object;
}

void MainInitialize()
{
    selected_object = nullptr;
    
    engine::GameObject* one   = engine::GameObjectManager::GetInstance().CreateGameObject("First");
    engine::GameObject* two   = engine::GameObjectManager::GetInstance().CreateGameObject("Second");
    engine::GameObject* three = engine::GameObjectManager::GetInstance().CreateGameObject("Third");

    one->AddComponent<ComponentA>();
    two->AddComponent<ComponentA>();
    three->AddComponent<ComponentA>();

    two->AddComponent<ComponentB>();
    three->AddComponent<ComponentB>();

    three->AddComponent<ComponentC>();

    one->Initialize();
    two->Initialize();
    three->Initialize();
}

void MainShutdown()
{
}

void MainDrawing()
{
    if (ImGui::BeginMainMenuBar())
    {
        ImGui::MenuItem("Dummy", nullptr, false, false);
        ImGui::EndMainMenuBar();
    }

    static ImGuiID dockIdLeft;
    static ImGuiID dockIdBottom;
    static ImGuiID dockIdRight;
    ImGuiID        dockSpaceId = dockSpaceId = ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton);

    static bool firstTime = true;
    if (firstTime)
    {
        firstTime = false;

        dockIdLeft   = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Left, 0.2f, nullptr, nullptr);
        dockIdBottom = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Down, 0.3f, nullptr, nullptr);
        dockIdRight  = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Right, 0.25f, nullptr, nullptr);
    }

    ImGui::SetNextWindowDockID(dockSpaceId, ImGuiCond_Appearing);
    if (ImGui::Begin("Scene"))
    {
        ImGui::Text("Scene");
    }
    ImGui::End();

    ImGui::SetNextWindowDockID(dockSpaceId, ImGuiCond_Appearing);
    if (ImGui::Begin("Game"))
    {
        ImGui::Text("Game");
    }
    ImGui::End();

    ImGui::SetNextWindowDockID(dockIdLeft, ImGuiCond_Appearing);
    if (ImGui::Begin("Hierarchy"))
    {
        ImGui::Text("Hierarchy");

        engine::GameObjectManager::GetInstance().ForEachRootGameObject(
            [](engine::GameObject* object) {
                ImGui::Text(object->GetName().c_str());

                if (ImGui::IsItemClicked())
                {
                    selected_object = object;
                }
            });
    }
    ImGui::End();

    ImGui::SetNextWindowDockID(dockIdBottom, ImGuiCond_Appearing);
    if (ImGui::Begin("Explorer"))
    {
        ImGui::Text("Explorer");
    }
    ImGui::End();

    ImGui::SetNextWindowDockID(dockIdBottom, ImGuiCond_Appearing);
    if (ImGui::Begin("Console"))
    {
        ImGui::Text("Console");
    }
    ImGui::End();

    ImGui::SetNextWindowDockID(dockIdRight, ImGuiCond_Appearing);
    if (ImGui::Begin("Properties"))
    {
        ImGui::Text("Properties");

        if (selected_object != nullptr)
        {
            ImGui::Text(selected_object->GetName().c_str());

            const std::list<engine::Component*>& components = selected_object->GetAllComponents();
            for (engine::Component* component : components)
            {
                ImGui::Text(typeid(*component).name());
            }
        }
    }
    ImGui::End();
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