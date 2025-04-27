#ifndef WINDOW_HPP
#define WINDOW_HPP

namespace engine
{
    class Window
    {
        struct Constants
        {
            static constexpr unsigned INITIAL_WINDOW_WIDTH  = 1280;
            static constexpr unsigned INITIAL_WINDOW_HEIGHT = 720;
        };

        void* m_window;
        void* m_context;

        bool m_isFullScreenEnabled;

        Window();
        ~Window() = default;

      public:
        Window(const Window&)                   = delete;
        Window&        operator=(const Window&) = delete;
        static Window& GetInstance();

        void Initialize();
        bool IsMinimized();
        void ProcessEvents(void* sdl_ev);
        void Shutdown();
        void SwapBuffers();

        void        SetTitle(const char* title) const;
        const char* GetTitle() const;

        void     SetSize(unsigned x, unsigned y);
        unsigned GetWidth() const;
        unsigned GetHeight() const;

        void SetFullScreenEnabled(bool enabled);
        bool IsFullScreenEnabled() const;

        void SetVSyncEnabled(bool enabled);
        bool IsVSyncEnabled() const;

        void* GetWindowHandle() const;
        void* GetContextHandle() const;
    };
} // namespace engine

#endif