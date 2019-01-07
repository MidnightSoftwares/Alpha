#pragma once

#include <queue>

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

class Mouse final
{
public:
    void PushMove(int x, int y);
    void PushRawMove(int x, int y);

    void PushLeftPress(int x, int y);
    void PushLeftRelease(int x, int y);

    void PushRightPress(int x, int y);
    void PushRightRelease(int x, int y);

    void PushMiddlePress(int x, int y);
    void PushMiddleRelease(int x, int y);

    void PushWheelUp(int x, int y);
    void PushWheelDown(int x, int y);

    MouseEvent PopEvent();

public:
    int PosX() const;
    int PosY() const;

    bool LeftPressed() const;
    bool RightPressed() const;
    bool MiddlePressed() const;

    bool EventQueueEmpty() const;

private:
    int mPosX = 0;
    int mPosY = 0;
    bool mLeftPressed = false;
    bool mRightPressed = false;
    bool mMiddlePressed = false;
    std::queue<MouseEvent> mEventQueue;
};
