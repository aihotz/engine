#include "input_manager.hpp"

#include <cctype> // tolower
#include <limits> // numeric_limits

#include <SDL3/SDL.h>

engine::InputManager::InputManager()
    : m_isAnythingTriggered { false }
    , m_isAnyKeyTriggered { false }
    , m_keyCount { 0 }
    , m_currentKeyboardState { nullptr }
    , m_previousKeyboardState { nullptr }
    , m_isMouseTriggered { false }
    , m_mouseScroll { 0.f }
    , m_currentMouseState { 0 }
    , m_previousMouseState { 0 }
    , m_currentMousePositionX { 0.0f }
    , m_currentMousePositionY { 0.0f }
    , m_previousMousePositionX { 0.0f }
    , m_previousMousePositionY { 0.0f }
    , m_isUsingController { false }
    , m_isControllerTriggered { false }
    , m_controller { nullptr }
    , m_currentControllerState { }
    , m_previousControllerState { }
    , m_gamepadAxes { }
{

}

engine::InputManager& engine::InputManager::GetInstance()
{
    static InputManager instance {};
    return instance;
}

void engine::InputManager::Initialize()
{
    // KEYBOARD
    // get the key count and allocate memory for it
    const bool* keyboardStatus = SDL_GetKeyboardState(&m_keyCount);
    m_currentKeyboardState     = new bool[ m_keyCount ];
    m_previousKeyboardState    = new bool[ m_keyCount ];

    // copy the state of the keyboard to the states
    memcpy(m_currentKeyboardState, keyboardStatus, m_keyCount);
    memcpy(m_previousKeyboardState, keyboardStatus, m_keyCount);

    // MOUSE
    // initialize data
    m_currentMouseState  = 0;
    m_previousMouseState = 0;

    m_currentMousePositionX  = 0.0f;
    m_currentMousePositionY  = 0.0f;
    m_previousMousePositionX = 0.0f;
    m_previousMousePositionY = 0.0f;

    // CONTROLLER
    m_isUsingController = true;
    m_controller        = nullptr;

    // initialize state arrays
    constexpr size_t controllerStateSize = static_cast<size_t>(sizeof(m_currentControllerState) / sizeof(m_currentControllerState[ 0 ]));
    for (size_t i = 0; i < controllerStateSize; i++)
    {
        m_currentControllerState[ i ]  = false;
        m_previousControllerState[ i ] = false;
    }

    constexpr size_t gamepadAxesSize = static_cast<size_t>(sizeof(m_gamepadAxes) / sizeof(m_gamepadAxes[ 0 ]));
    for (size_t i = 0; i < gamepadAxesSize; i++)
    {
        m_gamepadAxes[ i ] = 0.0f;
    }
}

void engine::InputManager::ConnectController()
{
    // disconnect the controller first if necessary
    if (m_controller)
    {
        DisconnectController();
    }

    // get the first connected controller
    m_controller = SDL_OpenGamepad(0);

    m_isUsingController = true;
}

void engine::InputManager::DisconnectController()
{
    if (m_controller)
    {
        SDL_CloseGamepad(static_cast<SDL_Gamepad*>(m_controller));
        m_controller = nullptr;
    }

    m_isUsingController = false;
}

void engine::InputManager::Update()
{
    SDL_PumpEvents();

    // KEYBOARD
    // get the current state of the keyboard
    const bool* keyboardState = SDL_GetKeyboardState(nullptr);

    // update the previous and current states of the keyboard
    memcpy(m_previousKeyboardState, m_currentKeyboardState, m_keyCount);
    memcpy(m_currentKeyboardState, keyboardState, m_keyCount);

    // determine if ANY key was triggered this frame
    m_isAnyKeyTriggered = false;
    for (int i = 0; i < m_keyCount; i++)
    {
        if (m_currentKeyboardState[ i ] == true && m_previousKeyboardState[ i ] == false)
        {
            m_isAnyKeyTriggered = true;
            break;
        }
    }

    // MOUSE
    // update the previous state of the mouse
    m_previousMouseState     = m_currentMouseState;
    m_previousMousePositionX = m_currentMousePositionX;
    m_previousMousePositionY = m_currentMousePositionY;

    // update the current state of the mouse
    m_currentMouseState = SDL_GetMouseState(&m_currentMousePositionX, &m_currentMousePositionY);

    // determine if ANY mouse button was triggered this frame
    m_isMouseTriggered = false;
    for (size_t i = 1; i < static_cast<size_t>(Mouse::MaxEnum); i++)
    {
        if ((m_currentMouseState & SDL_BUTTON_MASK(i)) && (m_previousMouseState & SDL_BUTTON_MASK(i)) == false)
        {
            m_isMouseTriggered = true;
            break;
        }
    }

    // CONTROLLER
    m_isControllerTriggered = false;

    // only update controller if one is present
    if (m_controller)
    {
        // update the previous state of the controller
        constexpr size_t controllerStateSize = static_cast<size_t>(sizeof(m_currentControllerState) / sizeof(m_currentControllerState[ 0 ]));
        for (size_t i = 0; i < controllerStateSize; i++)
        {
            m_previousControllerState[ i ] = m_currentControllerState[ i ];
        }

        // get the current state of controller's axes
        for (size_t i = 0; i < static_cast<size_t>(ControllerAxis::MaxEnum); i++)
        {
            // SDL_GetGamepadAxis returns a number in the range
            // [-32768,32767], or [-int16_t::max, int16_t::max]
            // (except for triggers, which are in range [0, 32767]
            // divide by int16_t::max to keep the numbers in range
            // [-1.0f, 1.0f] (or [0.0f, 1.0f] in the case of triggers).
            constexpr int16_t INT16_MAX_VALUE = std::numeric_limits<int16_t>().max();

            int16_t value = SDL_GetGamepadAxis(static_cast<SDL_Gamepad*>(m_controller), static_cast<SDL_GamepadAxis>(i));

            m_gamepadAxes[ i ] = value / static_cast<float>(INT16_MAX_VALUE);
        }

        // get the current state of the controller
        // triggers information is computed separately
        for (size_t i = 0; i < controllerStateSize; i++)
        {
            m_currentControllerState[ i ] = SDL_GetGamepadButton(static_cast<SDL_Gamepad*>(m_controller), static_cast<SDL_GamepadButton>(i));
        }

        // check if any button was triggered this frame
        for (size_t i = 0; i < controllerStateSize; i++)
        {
            if (m_currentControllerState[ i ] == true && m_previousControllerState[ i ] == false)
            {
                m_isControllerTriggered = true;
                break;
            }
        }
    }

    // check if anything was triggered this frame
    m_isAnythingTriggered = m_isAnyKeyTriggered || m_isMouseTriggered || m_isControllerTriggered;

    // check if the controller was the last thing in use

    // prevent a direct assignment to avoid changing the state
    // of m_isUsingController if nothing is triggered
    if (m_isAnyKeyTriggered || m_isMouseTriggered)
    {
        m_isUsingController = false;
    }

    if (m_controller)
    {
        if (m_isControllerTriggered)
        {
            m_isUsingController = true;
        }

        // do not check if we are already using the controller
        if (m_isUsingController == false)
        {
            // check if any axis is beyond the death zone
            // if an axis is moved, nothing is triggered,
            // but the controller should be considered in use

            constexpr size_t gamepadAxesSize = static_cast<size_t>(sizeof(m_gamepadAxes) / sizeof(m_gamepadAxes[ 0 ]));
            for (size_t i = 0; i < gamepadAxesSize; i++)
            {
                if (fabsf(m_gamepadAxes[ i ]) > Constants::CONTROLLER_DEAD_ZONE)
                {
                    m_isUsingController = true;
                    break;
                }
            }
        }
    }
}

void engine::InputManager::ProcessEvents(void* sdl_ev)
{
    SDL_Event* ev = static_cast<SDL_Event*>(sdl_ev);

    switch (ev->type)
    {
        case SDL_EVENT_GAMEPAD_ADDED: ConnectController(); break;
        case SDL_EVENT_GAMEPAD_REMOVED: DisconnectController(); break;
        case SDL_EVENT_MOUSE_WHEEL: m_mouseScroll = ev->wheel.y; break;
    }
}

void engine::InputManager::Shutdown()
{
    delete[] m_previousKeyboardState;
    m_previousKeyboardState = nullptr;

    delete[] m_currentKeyboardState;
    m_currentKeyboardState = nullptr;

    DisconnectController();
}

float engine::InputManager::GetLeftJoystickX() const
{
    return GetAxis(ControllerAxis::LeftX);
}

float engine::InputManager::GetLeftJoystickY() const
{
    return -GetAxis(ControllerAxis::LeftY);
}

float engine::InputManager::GetRightJoystickX() const
{
    return GetAxis(ControllerAxis::RightX);
}

float engine::InputManager::GetRightJoystickY() const
{
    return -GetAxis(ControllerAxis::RightY);
}

float engine::InputManager::GetAxis(ControllerAxis axis) const
{
    if (m_controller == nullptr)
    {
        return 0.0f;
    }

    // only return values of the axis past the dead zone
    float value = m_gamepadAxes[ static_cast<size_t>(axis) ];
    if (fabsf(value) > Constants::CONTROLLER_DEAD_ZONE)
    {
        return value;
    }

    return 0.0f;
}

bool engine::InputManager::IsTriggered(int key) const
{
    Keyboard keyCode = static_cast<Keyboard>(SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(std::tolower(key)), nullptr));
    return IsTriggered(keyCode);
}

bool engine::InputManager::IsTriggered(Keyboard key) const
{
    return m_currentKeyboardState[ static_cast<size_t>(key) ] == true && m_previousKeyboardState[ static_cast<size_t>(key) ] == false;
}

bool engine::InputManager::IsTriggered(Controller button) const
{
    if (m_controller == nullptr)
    {
        return false;
    }

    return m_currentControllerState[ static_cast<size_t>(button) ] == true && m_previousControllerState[ static_cast<size_t>(button) ] == false;
}

bool engine::InputManager::IsTriggered(Mouse button) const
{
    int mask = SDL_BUTTON_MASK(static_cast<size_t>(button));

    return (m_currentMouseState & mask) && (m_previousMouseState & mask) == false;
}

bool engine::InputManager::IsPressed(int key) const
{
    Keyboard keyCode = static_cast<Keyboard>(SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(std::tolower(key)), nullptr));
    return IsPressed(keyCode);
}

bool engine::InputManager::IsPressed(Keyboard key) const
{
    return m_currentKeyboardState[ static_cast<size_t>(key) ];
}

bool engine::InputManager::IsPressed(Controller button) const
{
    if (m_controller == nullptr)
    {
        return false;
    }

    return m_currentControllerState[ static_cast<size_t>(button) ] == true;
}

bool engine::InputManager::IsPressed(Mouse button) const
{
    int mask = SDL_BUTTON_MASK(static_cast<size_t>(button));

    return (m_currentMouseState & mask);
}

bool engine::InputManager::IsReleased(int key) const
{
    Keyboard keyCode = static_cast<Keyboard>(SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(std::tolower(key)), nullptr));
    return IsReleased(keyCode);
}

bool engine::InputManager::IsReleased(Keyboard key) const
{
    return m_currentKeyboardState[ static_cast<size_t>(key) ] == false && m_previousKeyboardState[ static_cast<size_t>(key) ] == true;
}

bool engine::InputManager::IsReleased(Controller button) const
{
    if (m_controller == nullptr)
    {
        return false;
    }

    return m_currentControllerState[ static_cast<size_t>(button) ] == false && m_previousControllerState[ static_cast<size_t>(button) ] == true;
}

bool engine::InputManager::IsReleased(Mouse button) const
{
    int mask = SDL_BUTTON_MASK(static_cast<size_t>(button));

    return (m_currentMouseState & mask) == false && (m_previousMouseState & mask);
}

float engine::InputManager::GetMousePositionX() const
{
    return m_currentMousePositionX;
}

float engine::InputManager::GetMousePositionY() const
{
    return m_currentMousePositionY;
}

float engine::InputManager::GetPreviousMousePositionX() const
{
    return m_previousMousePositionX;
}

float engine::InputManager::GetPreviousMousePositionY() const
{
    return m_previousMousePositionY;
}

float engine::InputManager::GetMouseMovementX() const
{
    return m_currentMousePositionX - m_previousMousePositionX;
}

float engine::InputManager::GetMouseMovementY() const
{
    return m_currentMousePositionY - m_previousMousePositionY;
}

float engine::InputManager::GetMouseScroll() const
{
    return m_mouseScroll;
}

bool engine::InputManager::IsAnythingTriggered() const
{
    return m_isAnythingTriggered;
}

bool engine::InputManager::IsKeyboardTriggered() const
{
    return m_isAnyKeyTriggered;
}

bool engine::InputManager::IsControllerTriggered() const
{
    return m_isControllerTriggered;
}

bool engine::InputManager::IsMouseTriggered() const
{
    return m_isMouseTriggered;
}

bool engine::InputManager::IsUsingController() const
{
    return m_isUsingController;
}

bool engine::InputManager::IsControllerConnected() const
{
    return m_controller != nullptr;
}

void engine::InputManager::VibrateController(float force, float duration) const
{
    VibrateControllerLowHigh(force, force, duration);
}

void engine::InputManager::VibrateControllerLowHigh(float low, float high, float duration) const
{
    if (m_controller == nullptr)
    {
        return;
    }

    // clamp if necessary
    if (low < 0.0f)
    {
        low = 0.0f;
    }
    else if (low > 1.0f)
    {
        low = 1.0f;
    }

    if (high < 0.0f)
    {
        high = 0.0f;
    }
    else if (high > 1.0f)
    {
        high = 1.0f;
    }

    // get the values in range 0x0000 to 0xFFFF
    constexpr uint16_t MAX_UINT16 = std::numeric_limits<uint16_t>::max();

    uint16_t lowForce  = static_cast<uint16_t>(low * MAX_UINT16);
    uint16_t highForce = static_cast<uint16_t>(high * MAX_UINT16);

    SDL_RumbleGamepad(static_cast<SDL_Gamepad*>(m_controller), lowForce, highForce, static_cast<uint32_t>(duration * 1000));
}