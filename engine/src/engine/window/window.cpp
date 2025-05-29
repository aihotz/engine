#include "window.hpp"

#include <stdexcept>

#include <SDL3/SDL.h>
#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>

void DebugCallback(
    gl::GLenum        source,
    gl::GLenum        type,
    gl::GLuint        id,
    gl::GLenum        severity,
    gl::GLsizei       length,
    const gl::GLchar* message,
    const void*       userParam)
{
    if (severity == gl::GL_DEBUG_SEVERITY_NOTIFICATION)
    {
        return;
    }

    const char* source_str = nullptr;
    const char* type_str = nullptr;
    const char* severity_str = nullptr;

    switch (source)
    {
        case gl::GL_DEBUG_SOURCE_API: source_str = "API"; break;
        case gl::GL_DEBUG_SOURCE_WINDOW_SYSTEM: source_str = "WINDOW SYSTEM"; break;
        case gl::GL_DEBUG_SOURCE_SHADER_COMPILER: source_str = "SHADER COMPILER"; break;
        case gl::GL_DEBUG_SOURCE_THIRD_PARTY: source_str = "THIRD PARTY"; break;
        case gl::GL_DEBUG_SOURCE_APPLICATION: source_str = "APPLICATION"; break;
        case gl::GL_DEBUG_SOURCE_OTHER: source_str = "UNKNOWN"; break;

        default: source_str = "UNKNOWN"; break;
    }

    switch (type)
    {
        case gl::GL_DEBUG_TYPE_ERROR: type_str = "ERROR"; break;
        case gl::GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "DEPRECATED BEHAVIOR"; break;
        case gl::GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: type_str = "UNDEFINED BEHAVIOR"; break;
        case gl::GL_DEBUG_TYPE_PORTABILITY: type_str = "PORTABILITY"; break;
        case gl::GL_DEBUG_TYPE_PERFORMANCE: type_str = "PERFORMANCE"; break;
        case gl::GL_DEBUG_TYPE_OTHER: type_str = "OTHER"; break;
        case gl::GL_DEBUG_TYPE_MARKER: type_str = "MARKER"; break;

        default: type_str = "UNKNOWN"; break;
    }

    switch (severity)
    {
        case gl::GL_DEBUG_SEVERITY_HIGH: severity_str = "HIGH"; break;
        case gl::GL_DEBUG_SEVERITY_MEDIUM: severity_str = "MEDIUM"; break;
        case gl::GL_DEBUG_SEVERITY_LOW: severity_str = "LOW"; break;
        case gl::GL_DEBUG_SEVERITY_NOTIFICATION: severity_str = "NOTIFICATION"; break;

        default: severity_str = "UNKNOWN"; break;
    }

    printf("%d: %s of %s severity, raised from %s: %s\n", id, type_str, severity_str, source_str, message);
}

engine::Window::Window()
    : m_window { nullptr }
    , m_context { nullptr }
    , m_isFullScreenEnabled { false }
{
}

engine::Window& engine::Window::GetInstance()
{
    static Window instance {};
    return instance;
}

void engine::Window::Initialize()
{
    // sdl init
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) == false)
    {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        throw std::runtime_error("SDL_Init failed");
    }

    // GL 4.6
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    // create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
    m_window           = SDL_CreateWindow("editor", Constants::INITIAL_WINDOW_WIDTH, Constants::INITIAL_WINDOW_HEIGHT, windowFlags);
    if (m_window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        throw std::runtime_error("SDL_Init failed");
    }

    SDL_SetWindowPosition(static_cast<SDL_Window*>(m_window), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    m_context = SDL_GL_CreateContext(static_cast<SDL_Window*>(m_window));
    if (m_context == nullptr)
    {
        printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
        throw std::runtime_error("SDL_Init failed");
    }

    SDL_GL_MakeCurrent(static_cast<SDL_Window*>(m_window), static_cast<SDL_GLContext>(m_context));
    SDL_ShowWindow(static_cast<SDL_Window*>(m_window));

    SetVSyncEnabled(true);
    SetFullScreenEnabled(m_isFullScreenEnabled);

    // todo: this goes into graphics module
    	glbinding::Binding::initialize(
        [](const char* name)
        { return reinterpret_cast<glbinding::ProcAddress>(SDL_GL_GetProcAddress(name)); }, false);

    gl::glEnable(gl::GL_DEBUG_OUTPUT);
    gl::glDebugMessageCallback(DebugCallback, 0);
}

bool engine::Window::IsMinimized()
{
    return SDL_GetWindowFlags(static_cast<SDL_Window*>(m_window)) & SDL_WINDOW_MINIMIZED;
}

void engine::Window::ProcessEvents(void* sdl_event)
{
    SDL_Event* ev = static_cast<SDL_Event*>(sdl_event);
    switch (ev->type)
    {
        case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
            m_isFullScreenEnabled = true;
            break;

        case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
            m_isFullScreenEnabled = false;
            break;
    }
}

void engine::Window::Shutdown()
{
    SDL_GL_DestroyContext(static_cast<SDL_GLContext>(m_context));
    SDL_DestroyWindow(static_cast<SDL_Window*>(m_window));
    SDL_Quit();
}

void engine::Window::SwapBuffers()
{
    SDL_GL_SwapWindow(static_cast<SDL_Window*>(m_window));
}

void engine::Window::SetTitle(const char* title) const
{
    if (title == nullptr)
    {
        throw std::runtime_error("Tried to set a null window title");
    }

    SDL_SetWindowTitle(static_cast<SDL_Window*>(m_window), title);
}

const char* engine::Window::GetTitle() const
{
    return SDL_GetWindowTitle(static_cast<SDL_Window*>(m_window));
}

void engine::Window::SetSize(unsigned x, unsigned y)
{
    if (x == 0 || y == 0)
    {
        return;
    }

    // note that SDL_SetWindowSize() might not set exactly the requested size
    SDL_SetWindowSize(static_cast<SDL_Window*>(m_window), static_cast<int>(x), static_cast<int>(y));
}

unsigned engine::Window::GetWidth() const
{
    int width;
    SDL_GetWindowSize(static_cast<SDL_Window*>(m_window), &width, nullptr);

    return static_cast<int>(width);
}

unsigned engine::Window::GetHeight() const
{
    int height;
    SDL_GetWindowSize(static_cast<SDL_Window*>(m_window), nullptr, &height);

    return static_cast<int>(height);
}

void engine::Window::SetFullScreenEnabled(bool enabled)
{
    // note that the effect of SDL_SetWindowFullscreen() might not be immediate
    // m_fullScreenEnabled is set in the ProcessEvents() method when the SDL_EVENT_WINDOW_ENTER_FULLSCREEN or SDL_EVENT_WINDOW_LEAVE_FULLSCREEN event is emitted
    SDL_SetWindowFullscreen(static_cast<SDL_Window*>(m_window), enabled);
}

bool engine::Window::IsFullScreenEnabled() const
{
    return m_isFullScreenEnabled;
}

void engine::Window::SetVSyncEnabled(bool enabled)
{
    // ignoring adaptive vsync 
    SDL_GL_SetSwapInterval(static_cast<int>(enabled));
}

bool engine::Window::IsVSyncEnabled() const
{
    int swapInterval;
    SDL_GL_GetSwapInterval(&swapInterval);

    return swapInterval == 1;
}

void* engine::Window::GetWindowHandle() const
{
    return m_window;
}

void* engine::Window::GetContextHandle() const
{
    return m_context;
}