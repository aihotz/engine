#ifndef WINDOW_HPP
#define WINDOW_HPP

namespace engine
{
    class Window
    {
        struct Constants
        {
            static constexpr unsigned INITIAL_WINDOW_WIDTH  = 1920;
            static constexpr unsigned INITIAL_WINDOW_HEIGHT = 1080;
        };

        void* m_window;
        void* m_context;

        bool m_isFullScreenEnabled;

        Window();
        ~Window()                        = default;
        Window(const Window&)            = delete;
        Window& operator=(const Window&) = delete;

      public:
        static Window& GetInstance();

        void Initialize();
        bool IsMinimized();
        void ProcessEvents(void* sdl_event);
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