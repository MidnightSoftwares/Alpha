#pragma once

#include <queue>

struct KeyEvent final
{
    enum Type
    {
        Press,
        Release
    };

    Type Type;
    unsigned char KeyCode;
};

struct MouseEvent final
{
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

    Type Type;
    int X;
    int Y;
};

class Input final
{
public:
    Input();

public:
    void PushKeyPress(unsigned char keyCode);
    void PushKeyRelease(unsigned char keyCode);
    KeyEvent PopKeyEvent();

    void PushChar(wchar_t ch);
    wchar_t PopChar();

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

    bool KeyEventQueueEmpty() const;
    bool CharQueueEmpty() const;

public:
    int MousePosX() const;
    int MousePosY() const;

    bool MouseLeftPressed() const;
    bool MouseRightPressed() const;
    bool MouseMiddlePressed() const;

    bool MouseEventQueueEmpty() const;

private:
    bool mAutoRepeatKeys = false;
    bool mAutoRepeatChars = false;
    bool mKeyPressed[UCHAR_MAX + 1] = {};
    std::queue<KeyEvent> mKeyEventQueue;
    std::queue<wchar_t> mCharQueue;
    int mMousePosX = 0;
    int mMousePosY = 0;
    bool mMouseLeftPressed = false;
    bool mMouseRightPressed = false;
    bool mMouseMiddlePressed = false;
    std::queue<MouseEvent> mMouseEventQueue;
};
