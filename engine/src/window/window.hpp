#ifndef WINDOW_HPP
#define WINDOW_HPP

namespace engine
{
    class Window //: public Singleton<Window>
    {
        void* m_window  = nullptr;
        void* m_context = nullptr;

      public:
        void     Initialize();
        void     Shutdown();
        void     SwapBuffers();
        unsigned GetWindowId() const;

        enum class FullScreenMode
        {
            Windowed,
            FullScreen,
            Borderless
        };

        void     SetTitle(const char* title) const;
        void     SetSize(unsigned x, unsigned y);
        void     SetFullScreenMode(FullScreenMode mode);
        unsigned GetWidth() const;
        unsigned GetHeight() const;
    };
} // namespace engine

#endif