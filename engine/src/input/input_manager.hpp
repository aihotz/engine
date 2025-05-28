#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include "key_codes.hpp" // Keyboard, Mouse, Controller, ControllerAxis enum classes

namespace engine
{
    class InputManager
    {
        struct Constants
        {
            static constexpr float CONTROLLER_DEAD_ZONE = 0.2f;
        };

        bool m_isAnythingTriggered;

        // KEYBOARD
        bool  m_isAnyKeyTriggered;
        int   m_keyCount;
        bool* m_currentKeyboardState;
        bool* m_previousKeyboardState;

        // MOUSE
        bool     m_isMouseTriggered;
        float    m_mouseScroll;
        unsigned m_currentMouseState;
        unsigned m_previousMouseState;
        float    m_currentMousePositionX;
        float    m_currentMousePositionY;
        float    m_previousMousePositionX;
        float    m_previousMousePositionY;

        // CONTROLLER
        bool  m_isUsingController;
        bool  m_isControllerTriggered;
        void* m_controller;
        bool  m_currentControllerState[ static_cast<size_t>(Controller::MaxEnum) ];
        bool  m_previousControllerState[ static_cast<size_t>(Controller::MaxEnum) ];
        float m_gamepadAxes[ static_cast<size_t>(ControllerAxis::MaxEnum) ];

        InputManager();
        ~InputManager()                              = default;
        InputManager(const InputManager&)            = delete;
        InputManager& operator=(const InputManager&) = delete;

        void ConnectController();
        void DisconnectController();

      public:
        static InputManager& GetInstance();

        void Initialize();
        void Update();
        void ProcessEvents(void* sdl_ev);
        void Shutdown();

        float GetLeftJoystickX() const;
        float GetLeftJoystickY() const;
        float GetRightJoystickX() const;
        float GetRightJoystickY() const;
        float GetAxis(ControllerAxis axis) const;

        bool IsTriggered(int key) const;
        bool IsTriggered(Keyboard key) const;
        bool IsTriggered(Controller button) const;
        bool IsTriggered(Mouse button) const;

        bool IsPressed(int key) const;
        bool IsPressed(Keyboard key) const;
        bool IsPressed(Controller button) const;
        bool IsPressed(Mouse button) const;

        bool IsReleased(int key) const;
        bool IsReleased(Keyboard key) const;
        bool IsReleased(Controller button) const;
        bool IsReleased(Mouse button) const;

        float GetMousePositionX() const;
        float GetMousePositionY() const;
        float GetPreviousMousePositionX() const;
        float GetPreviousMousePositionY() const;
        float GetMouseMovementX() const;
        float GetMouseMovementY() const;
        float GetMouseScroll() const;

        bool IsAnythingTriggered() const;
        bool IsKeyboardTriggered() const;
        bool IsControllerTriggered() const;
        bool IsMouseTriggered() const;
        bool IsUsingController() const;

        bool IsControllerConnected() const;
        void VibrateController(float force = 1.0f, float duration = 1.0f) const;
        void VibrateControllerLowHigh(float low = 1.0f, float high = 1.0f, float duration = 1.0f) const;
    };
} // namespace engine

#endif