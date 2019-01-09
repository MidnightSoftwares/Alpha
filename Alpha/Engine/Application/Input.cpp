#include "Input.h"
#include "../Utils/Debug.h"

KeyEvent::KeyEvent(enum Type type, unsigned char keyCode):
    Type{type},
    KeyCode{keyCode}
{
}

MouseEvent::MouseEvent(enum Type type, int x, int y):
    Type{type},
    X{x},
    Y{y}
{
}

void Input::PushKeyPress(unsigned char keyCode)
{
    mKeyPressed[keyCode] = true;
    mKeyEvents.emplace(KeyEvent::Press, keyCode);
}

void Input::PushKeyRelease(unsigned char keyCode)
{
    mKeyPressed[keyCode] = false;
    mKeyEvents.emplace(KeyEvent::Release, keyCode);
}

KeyEvent Input::PopKeyEvent()
{
    ASSERT(!mKeyEvents.empty());

    const auto keyEvent = mKeyEvents.front();
    mKeyEvents.pop();

    return keyEvent;
}

void Input::PushCharInput(wchar_t ch)
{
    mCharInputs.push(ch);
}

wchar_t Input::PopCharInput()
{
    ASSERT(!mCharInputs.empty());

    const auto ch = mCharInputs.front();
    mCharInputs.pop();

    return ch;
}

void Input::PushMouseMove(int x, int y)
{
    mMousePosX = x;
    mMousePosY = y;
    mMouseEvents.emplace(MouseEvent::Move, x, y);
}

void Input::PushMouseRawMove(int x, int y)
{
    mMouseEvents.emplace(MouseEvent::RawMove, x, y);
}

void Input::PushMouseLeftPress(int x, int y)
{
    mMouseLeftPressed = true;
    mMouseEvents.emplace(MouseEvent::LPress, x, y);
}

void Input::PushMouseLeftRelease(int x, int y)
{
    mMouseLeftPressed = false;
    mMouseEvents.emplace(MouseEvent::LRelease, x, y);
}

void Input::PushMouseRightPress(int x, int y)
{
    mMouseRightPressed = true;
    mMouseEvents.emplace(MouseEvent::RPress, x, y);
}

void Input::PushMouseRightRelease(int x, int y)
{
    mMouseRightPressed = false;
    mMouseEvents.emplace(MouseEvent::RRelease, x, y);
}

void Input::PushMouseMiddlePress(int x, int y)
{
    mMouseMiddlePressed = true;
    mMouseEvents.emplace(MouseEvent::MPress, x, y);
}

void Input::PushMouseMiddleRelease(int x, int y)
{
    mMouseMiddlePressed = false;
    mMouseEvents.emplace(MouseEvent::MRelease, x, y);
}

void Input::PushMouseWheelUp(int x, int y)
{
    mMouseEvents.emplace(MouseEvent::WheelUp, x, y);
}

void Input::PushMouseWheelDown(int x, int y)
{
    mMouseEvents.emplace(MouseEvent::WheelDown, x, y);
}

MouseEvent Input::PopMouseEvent()
{
    ASSERT(!mMouseEvents.empty());

    const auto mouseEvent = mMouseEvents.front();
    mMouseEvents.pop();

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

bool Input::KeyEventsEmpty() const
{
    return mKeyEvents.empty();
}

bool Input::CharsInputsEmpty() const
{
    return mCharInputs.empty();
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

bool Input::MouseEventsEmpty() const
{
    return mMouseEvents.empty();
}
