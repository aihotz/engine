#ifndef KEY_CODES_HPP
#define KEY_CODES_HPP

namespace engine
{
    enum class Keyboard : size_t
    {
        A = 4, //= SDL_SCANCODE_A,
        B,     //= SDL_SCANCODE_B,
        C,     //= SDL_SCANCODE_C,
        D,     //= SDL_SCANCODE_D,
        E,     //= SDL_SCANCODE_E,
        F,     //= SDL_SCANCODE_F,
        G,     //= SDL_SCANCODE_G,
        H,     //= SDL_SCANCODE_H,
        I,     //= SDL_SCANCODE_I,
        J,     //= SDL_SCANCODE_J,
        K,     //= SDL_SCANCODE_K,
        L,     //= SDL_SCANCODE_L,
        M,     //= SDL_SCANCODE_M,
        N,     //= SDL_SCANCODE_N,
        O,     //= SDL_SCANCODE_O,
        P,     //= SDL_SCANCODE_P,
        Q,     //= SDL_SCANCODE_Q,
        R,     //= SDL_SCANCODE_R,
        S,     //= SDL_SCANCODE_S,
        T,     //= SDL_SCANCODE_T,
        U,     //= SDL_SCANCODE_U,
        V,     //= SDL_SCANCODE_V,
        W,     //= SDL_SCANCODE_W,
        X,     //= SDL_SCANCODE_X,
        Y,     //= SDL_SCANCODE_Y,
        Z,     //= SDL_SCANCODE_Z,

        Num1, //= SDL_SCANCODE_1,
        Num2, //= SDL_SCANCODE_2,
        Num3, //= SDL_SCANCODE_3,
        Num4, //= SDL_SCANCODE_4,
        Num5, //= SDL_SCANCODE_5,
        Num6, //= SDL_SCANCODE_6,
        Num7, //= SDL_SCANCODE_7,
        Num8, //= SDL_SCANCODE_8,
        Num9, //= SDL_SCANCODE_9,
        Num0, //= SDL_SCANCODE_0,

        Return,    //= SDL_SCANCODE_RETURN,
        Escape,    //= SDL_SCANCODE_ESCAPE,
        Backspace, //= SDL_SCANCODE_BACKSPACE,
        Tab,       //= SDL_SCANCODE_TAB,
        Space,     //= SDL_SCANCODE_SPACE,

        F1 = 58, //= SDL_SCANCODE_F1,
        F2,      //= SDL_SCANCODE_F2,
        F3,      //= SDL_SCANCODE_F3,
        F4,      //= SDL_SCANCODE_F4,
        F5,      //= SDL_SCANCODE_F5,
        F6,      //= SDL_SCANCODE_F6,
        F7,      //= SDL_SCANCODE_F7,
        F8,      //= SDL_SCANCODE_F8,
        F9,      //= SDL_SCANCODE_F9,
        F10,     //= SDL_SCANCODE_F10,
        F11,     //= SDL_SCANCODE_F11,
        F12,     //= SDL_SCANCODE_F12,

        Right = 79, //= SDL_SCANCODE_RIGHT,
        Left,       //= SDL_SCANCODE_LEFT,
        Down,       //= SDL_SCANCODE_DOWN,
        Up,         //= SDL_SCANCODE_UP,

        LCtrl  = 224, //= SDL_SCANCODE_LCTRL,
        LShift = 225, //= SDL_SCANCODE_LSHIFT,
        RCtrl  = 228, //= SDL_SCANCODE_RCTRL,
        RShift = 229, //= SDL_SCANCODE_RSHIFT,
    };

    enum class Controller : size_t
    {
        A,             //= SDL_GAMEPAD_BUTTON_SOUTH,
        B,             //= SDL_GAMEPAD_BUTTON_EAST,
        X,             //= SDL_GAMEPAD_BUTTON_WEST,
        Y,             //= SDL_GAMEPAD_BUTTON_NORTH,
        Back,          //= SDL_GAMEPAD_BUTTON_BACK,
        Guide,         //= SDL_GAMEPAD_BUTTON_GUIDE,
        Start,         //= SDL_GAMEPAD_BUTTON_START,
        LeftStick,     //= SDL_GAMEPAD_BUTTON_LEFT_STICK,
        RightStick,    //= SDL_GAMEPAD_BUTTON_RIGHT_STICK,
        LeftShoulder,  //= SDL_GAMEPAD_BUTTON_LEFT_SHOULDER,
        RightShoulder, //= SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER,
        Up,            //= SDL_GAMEPAD_BUTTON_DPAD_UP,
        Down,          //= SDL_GAMEPAD_BUTTON_DPAD_DOWN,
        Left,          //= SDL_GAMEPAD_BUTTON_DPAD_LEFT,
        Right,         //= SDL_GAMEPAD_BUTTON_DPAD_RIGHT,

        Misc1,    //= SDL_GAMEPAD_BUTTON_MISC1,         /* Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button */
        Paddle1,  //= SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1, /* Xbox Elite paddle P1 */
        Paddle2,  //= SDL_GAMEPAD_BUTTON_LEFT_PADDLE1,  /* Xbox Elite paddle P2 */
        Paddle3,  //= SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2, /* Xbox Elite paddle P3 */
        Paddle4,  //= SDL_GAMEPAD_BUTTON_LEFT_PADDLE2,  /* Xbox Elite paddle P4 */
        Touchpad, //= SDL_GAMEPAD_BUTTON_TOUCHPAD,      /* PS4/PS5 touchpad button */

        Misc2, //= SDL_GAMEPAD_BUTTON_MISC2,
        Misc3, //= SDL_GAMEPAD_BUTTON_MISC3,
        Misc4, //= SDL_GAMEPAD_BUTTON_MISC4,
        Misc5, //= SDL_GAMEPAD_BUTTON_MISC5,
        Misc6, //= SDL_GAMEPAD_BUTTON_MISC6,

        // in general, this will be handled by ControllerAxis
        //LeftTrigger,
        //RightTrigger,

        MaxEnum // = SDL_GAMEPAD_BUTTON_COUNT
    };

    enum class ControllerAxis : size_t
    {
        LeftX,        //= SDL_GAMEPAD_AXIS_LEFTX,
        LeftY,        //= SDL_GAMEPAD_AXIS_LEFTY,
        RightX,       //= SDL_GAMEPAD_AXIS_RIGHTX,
        RightY,       //= SDL_GAMEPAD_AXIS_RIGHTY,
        LeftTrigger,  //= SDL_GAMEPAD_AXIS_LEFT_TRIGGER,
        RightTrigger, //= SDL_GAMEPAD_AXIS_RIGHT_TRIGGER,
        MaxEnum,      //= SDL_GAMEPAD_AXIS_COUNT
    };

    enum class Mouse : size_t
    {
        Left = 1, //= SDL_BUTTON_LEFT,
        Middle,   //= SDL_BUTTON_MIDDLE,
        Right,    //= SDL_BUTTON_RIGHT,
        MB4,      //= SDL_BUTTON_X1,
        MB5,      //= SDL_BUTTON_X2,

        MaxEnum
    };
} // namespace engine

#endif