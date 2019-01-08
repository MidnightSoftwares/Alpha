#include "Input.h"
#include "../Utils/Debug.h"

Input::Input()
{
    for (auto& keyPressed : mKeyPressed)
    {
        keyPressed = false;
    }
}

void Input::PushKeyPress(unsigned char keyCode)
{
    mKeyPressed[keyCode] = true;
    mKeyEventQueue.push({KeyEvent::Press, keyCode});
}

void Input::PushKeyRelease(unsigned char keyCode)
{
    mKeyPressed[keyCode] = false;
    mKeyEventQueue.push({KeyEvent::Release, keyCode});
}

KeyEvent Input::PopKeyEvent()
{
    ASSERT(!mKeyEventQueue.empty());

    const auto keyEvent = mKeyEventQueue.front();
    mKeyEventQueue.pop();
    return keyEvent;
}

void Input::PushChar(wchar_t ch)
{
    mCharQueue.push(ch);
}

wchar_t Input::PopChar()
{
    ASSERT(!mCharQueue.empty());

    const auto ch = mCharQueue.front();
    mCharQueue.pop();
    return ch;
}

void Input::PushMouseMove(int x, int y)
{
    mMousePosX = x;
    mMousePosY = y;
    mMouseEventQueue.push({MouseEvent::Move, x, y});
}

void Input::PushMouseRawMove(int x, int y)
{
    mMouseEventQueue.push({MouseEvent::RawMove, x, y});
}

void Input::PushMouseLeftPress(int x, int y)
{
    mMouseLeftPressed = true;
    mMouseEventQueue.push({MouseEvent::LPress, x, y});
}

void Input::PushMouseLeftRelease(int x, int y)
{
    mMouseLeftPressed = false;
    mMouseEventQueue.push({MouseEvent::LRelease, x, y});
}

void Input::PushMouseRightPress(int x, int y)
{
    mMouseRightPressed = true;
    mMouseEventQueue.push({MouseEvent::RPress, x, y});
}

void Input::PushMouseRightRelease(int x, int y)
{
    mMouseRightPressed = false;
    mMouseEventQueue.push({MouseEvent::RRelease, x, y});
}

void Input::PushMouseMiddlePress(int x, int y)
{
    mMouseMiddlePressed = true;
    mMouseEventQueue.push({MouseEvent::MPress, x, y});
}

void Input::PushMouseMiddleRelease(int x, int y)
{
    mMouseMiddlePressed = false;
    mMouseEventQueue.push({MouseEvent::MRelease, x, y});
}

void Input::PushMouseWheelUp(int x, int y)
{
    mMouseEventQueue.push({MouseEvent::WheelUp, x, y});
}

void Input::PushMouseWheelDown(int x, int y)
{
    mMouseEventQueue.push({MouseEvent::WheelDown, x, y});
}

MouseEvent Input::PopMouseEvent()
{
    ASSERT(!mMouseEventQueue.empty());

    const auto mouseEvent = mMouseEventQueue.front();
    mMouseEventQueue.pop();
    return mouseEvent;
}

bool Input::AutoRepeatKeys() const
{
    return mAutoRepeatKeys;
}

void Input::SetAutoRepeatKeys(bool autoRepeatKeys)
{
    mAutoRepeatKeys = autoRepeatKeys;
}

bool Input::AutoRepeatChars() const
{
    return mAutoRepeatChars;
}

void Input::SetAutoRepeatChars(bool autoRepeatChars)
{
    mAutoRepeatChars = autoRepeatChars;
}

bool Input::KeyPressed(unsigned char keyCode) const
{
    return mKeyPressed[keyCode];
}

bool Input::KeyEventQueueEmpty() const
{
    return mKeyEventQueue.empty();
}

bool Input::CharQueueEmpty() const
{
    return mCharQueue.empty();
}

int Input::MousePosX() const
{
    return mMousePosX;
}

int Input::MousePosY() const
{
    return mMousePosY;
}

bool Input::MouseLeftPressed() const
{
    return mMouseLeftPressed;
}

bool Input::MouseRightPressed() const
{
    return mMouseRightPressed;
}

bool Input::MouseMiddlePressed() const
{
    return mMouseMiddlePressed;
}

bool Input::MouseEventQueueEmpty() const
{
    return mMouseEventQueue.empty();
}
