#include "Keyboard.h"
#include "../Utils/Debug.h"

Keyboard::Keyboard()
{
    for (auto& keyPressed : mKeyPressed)
    {
        keyPressed = false;
    }
}

void Keyboard::PushKeyPress(unsigned char keyCode)
{
    mKeyPressed[keyCode] = true;
    mKeyQueue.push({KeyEvent::Press, keyCode});
}

void Keyboard::PushKeyRelease(unsigned char keyCode)
{
    mKeyPressed[keyCode] = false;
    mKeyQueue.push({KeyEvent::Release, keyCode});
}

KeyEvent Keyboard::PopKey()
{
    ASSERT(!mKeyQueue.empty());

    const auto keyEvent = mKeyQueue.front();
    mKeyQueue.pop();
    return keyEvent;
}

void Keyboard::PushChar(unsigned char ch)
{
    mCharQueue.push(ch);
}

unsigned char Keyboard::PopChar()
{
    ASSERT(!mCharQueue.empty());

    const auto ch = mCharQueue.front();
    mCharQueue.pop();
    return ch;
}

bool Keyboard::AutoRepeatKeys() const
{
    return mAutoRepeatKeys;
}

void Keyboard::SetAutoRepeatKeys(bool autoRepeatKeys)
{
    mAutoRepeatKeys = autoRepeatKeys;
}

bool Keyboard::AutoRepeatChars() const
{
    return mAutoRepeatChars;
}

void Keyboard::SetAutoRepeatChars(bool autoRepeatChars)
{
    mAutoRepeatChars = autoRepeatChars;
}

bool Keyboard::KeyPressed(unsigned char keyCode) const
{
    return mKeyPressed[keyCode];
}

bool Keyboard::KeyQueueEmpty() const
{
    return mKeyQueue.empty();
}

bool Keyboard::CharQueueEmpty() const
{
    return mCharQueue.empty();
}
