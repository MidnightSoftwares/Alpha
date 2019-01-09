#pragma once

#include <array>
#include <queue>

struct KeyEvent final
{
public:
    enum Type
    {
        Press,
        Release
    };

public:
    KeyEvent(Type type, unsigned char keyCode);

public:
    Type Type;
    unsigned char KeyCode;
};

struct MouseEvent final
{
public:
    enum Type
    {
        Move,
        RawMove,
        LPress,
        LRelease,
        RPress,
        RRelease,
        MPress,
        MRelease,
        WheelUp,
        WheelDown
    };

public:
    MouseEvent(Type type, int x, int y);

public:
    Type Type;
    int X;
    int Y;
};

class Input
{
public:
    void PushKeyPress(unsigned char keyCode);
    void PushKeyRelease(unsigned char keyCode);
    KeyEvent PopKeyEvent();

    void PushCharInput(wchar_t ch);
    wchar_t PopCharInput();

public:
    void PushMouseMove(int x, int y);
    void PushMouseRawMove(int x, int y);

    void PushMouseLeftPress(int x, int y);
    void PushMouseLeftRelease(int x, int y);

    void PushMouseRightPress(int x, int y);
    void PushMouseRightRelease(int x, int y);

    void PushMouseMiddlePress(int x, int y);
    void PushMouseMiddleRelease(int x, int y);

    void PushMouseWheelUp(int x, int y);
    void PushMouseWheelDown(int x, int y);

    MouseEvent PopMouseEvent();

public:
    bool AutoRepeatKeys() const;
    void SetAutoRepeatKeys(bool autoRepeatKeys);

    bool AutoRepeatChars() const;
    void SetAutoRepeatChars(bool autoRepeatChars);

    bool KeyPressed(unsigned char keyCode) const;

    bool KeyEventsEmpty() const;

    bool CharsInputsEmpty() const;

public:
    int MousePosX() const;
    int MousePosY() const;

    bool MouseLeftPressed() const;
    bool MouseRightPressed() const;
    bool MouseMiddlePressed() const;

    bool MouseEventsEmpty() const;

private:
    bool mAutoRepeatKeys = false;
    bool mAutoRepeatChars = false;
    std::array<bool, UCHAR_MAX + 1> mKeyPressed{};
    std::queue<KeyEvent> mKeyEvents;
    std::queue<wchar_t> mCharInputs;
    int mMousePosX = 0;
    int mMousePosY = 0;
    bool mMouseLeftPressed = false;
    bool mMouseRightPressed = false;
    bool mMouseMiddlePressed = false;
    std::queue<MouseEvent> mMouseEvents;
};
