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
    KeyEvent(enum Type type, unsigned char keyCode);

public:
    Type Type;
    unsigned char KeyCode;
};

class KeyboardInput final
{
public:
    void PushKeyPress(unsigned char keyCode);
    void PushKeyRelease(unsigned char keyCode);
    KeyEvent PopKeyEvent();

    void PushCharInput(wchar_t ch);
    wchar_t PopCharInput();

public:
    bool AutoRepeatKeys() const;
    void SetAutoRepeatKeys(bool autoRepeatKeys);

    bool AutoRepeatChars() const;
    void SetAutoRepeatChars(bool autoRepeatChars);

    bool KeyPressed(unsigned char keyCode) const;

    bool KeyEventsEmpty() const;

    bool CharsInputsEmpty() const;

private:
    bool mAutoRepeatKeys = false;
    bool mAutoRepeatChars = false;
    std::array<bool, UCHAR_MAX + 1> mKeyPressed{};
    std::queue<KeyEvent> mKeyEvents;
    std::queue<wchar_t> mCharInputs;
};
