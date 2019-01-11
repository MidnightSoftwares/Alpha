#include "KeyboardInput.h"
#include "../Utils/Debug.h"

KeyEvent::KeyEvent(enum Type type, unsigned char keyCode):
    Type{type},
    KeyCode{keyCode}
{
}

void KeyboardInput::PushKeyPress(unsigned char keyCode)
{
    mKeyPressed[keyCode] = true;
    mKeyEvents.emplace(KeyEvent::Press, keyCode);
}

void KeyboardInput::PushKeyRelease(unsigned char keyCode)
{
    mKeyPressed[keyCode] = false;
    mKeyEvents.emplace(KeyEvent::Release, keyCode);
}

KeyEvent KeyboardInput::PopKeyEvent()
{
    ASSERT(!mKeyEvents.empty());

    const auto keyEvent = mKeyEvents.front();
    mKeyEvents.pop();

    return keyEvent;
}

void KeyboardInput::PushCharInput(wchar_t ch)
{
    mCharInputs.push(ch);
}

wchar_t KeyboardInput::PopCharInput()
{
    ASSERT(!mCharInputs.empty());

    const auto ch = mCharInputs.front();
    mCharInputs.pop();

    return ch;
}

bool KeyboardInput::AutoRepeatKeys() const
{
    return mAutoRepeatKeys;
}

void KeyboardInput::SetAutoRepeatKeys(bool autoRepeatKeys)
{
    mAutoRepeatKeys = autoRepeatKeys;
}

bool KeyboardInput::AutoRepeatChars() const
{
    return mAutoRepeatChars;
}

void KeyboardInput::SetAutoRepeatChars(bool autoRepeatChars)
{
    mAutoRepeatChars = autoRepeatChars;
}

bool KeyboardInput::KeyPressed(unsigned char keyCode) const
{
    return mKeyPressed[keyCode];
}

bool KeyboardInput::KeyEventsEmpty() const
{
    return mKeyEvents.empty();
}

bool KeyboardInput::CharsInputsEmpty() const
{
    return mCharInputs.empty();
}
