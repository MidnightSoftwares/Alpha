#include "MouseInput.h"
#include "../Utils/Debug.h"

MouseEvent::MouseEvent(enum Type type, int x, int y):
    Type{type},
    X{x},
    Y{y}
{
}

void MouseInput::PushMove(int x, int y)
{
    mPosX = x;
    mPosY = y;
    mEvents.emplace(MouseEvent::Move, x, y);
}

void MouseInput::PushRawMove(int x, int y)
{
    mEvents.emplace(MouseEvent::RawMove, x, y);
}

void MouseInput::PushLeftPress(int x, int y)
{
    mLeftPressed = true;
    mEvents.emplace(MouseEvent::LPress, x, y);
}

void MouseInput::PushLeftRelease(int x, int y)
{
    mLeftPressed = false;
    mEvents.emplace(MouseEvent::LRelease, x, y);
}

void MouseInput::PushRightPress(int x, int y)
{
    mRightPressed = true;
    mEvents.emplace(MouseEvent::RPress, x, y);
}

void MouseInput::PushRightRelease(int x, int y)
{
    mRightPressed = false;
    mEvents.emplace(MouseEvent::RRelease, x, y);
}

void MouseInput::PushMiddlePress(int x, int y)
{
    mMiddlePressed = true;
    mEvents.emplace(MouseEvent::MPress, x, y);
}

void MouseInput::PushMiddleRelease(int x, int y)
{
    mMiddlePressed = false;
    mEvents.emplace(MouseEvent::MRelease, x, y);
}

void MouseInput::PushWheelUp(int x, int y)
{
    mEvents.emplace(MouseEvent::WheelUp, x, y);
}

void MouseInput::PushWheelDown(int x, int y)
{
    mEvents.emplace(MouseEvent::WheelDown, x, y);
}

MouseEvent MouseInput::PopEvent()
{
    ASSERT(!mEvents.empty());

    const auto mouseEvent = mEvents.front();
    mEvents.pop();

    return mouseEvent;
}

int MouseInput::PosX() const
{
    return mPosX;
}

int MouseInput::PosY() const
{
    return mPosY;
}

bool MouseInput::LeftPressed() const
{
    return mLeftPressed;
}

bool MouseInput::RightPressed() const
{
    return mRightPressed;
}

bool MouseInput::MiddlePressed() const
{
    return mMiddlePressed;
}

bool MouseInput::EventsEmpty() const
{
    return mEvents.empty();
}
