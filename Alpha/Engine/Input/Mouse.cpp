#include "Mouse.h"
#include "../Utils/Debug.h"

void Mouse::PushMove(int x, int y)
{
    mPosX = x;
    mPosY = y;
    mEventQueue.push({MouseEvent::Move, x, y});
}

void Mouse::PushRawMove(int x, int y)
{
    mEventQueue.push({MouseEvent::RawMove, x, y});
}

void Mouse::PushLeftPress(int x, int y)
{
    mLeftPressed = true;
    mEventQueue.push({MouseEvent::LPress, x, y});
}

void Mouse::PushLeftRelease(int x, int y)
{
    mLeftPressed = false;
    mEventQueue.push({MouseEvent::LRelease, x, y});
}

void Mouse::PushRightPress(int x, int y)
{
    mRightPressed = true;
    mEventQueue.push({MouseEvent::RPress, x, y});
}

void Mouse::PushRightRelease(int x, int y)
{
    mRightPressed = false;
    mEventQueue.push({MouseEvent::RRelease, x, y});
}

void Mouse::PushMiddlePress(int x, int y)
{
    mMiddlePressed = true;
    mEventQueue.push({MouseEvent::MPress, x, y});
}

void Mouse::PushMiddleRelease(int x, int y)
{
    mMiddlePressed = false;
    mEventQueue.push({MouseEvent::MRelease, x, y});
}

void Mouse::PushWheelUp(int x, int y)
{
    mEventQueue.push({MouseEvent::WheelUp, x, y});
}

void Mouse::PushWheelDown(int x, int y)
{
    mEventQueue.push({MouseEvent::WheelDown, x, y});
}

MouseEvent Mouse::PopEvent()
{
    ASSERT(!mEventQueue.empty());

    const auto mouseEvent = mEventQueue.front();
    mEventQueue.pop();
    return mouseEvent;
}

int Mouse::PosX() const
{
    return mPosX;
}

int Mouse::PosY() const
{
    return mPosY;
}

bool Mouse::LeftPressed() const
{
    return mLeftPressed;
}

bool Mouse::RightPressed() const
{
    return mRightPressed;
}

bool Mouse::MiddlePressed() const
{
    return mMiddlePressed;
}

bool Mouse::EventQueueEmpty() const
{
    return mEventQueue.empty();
}
