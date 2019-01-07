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

class Keyboard final
{
public:
    Keyboard();

public:
    void PushKeyPress(unsigned char keyCode);
    void PushKeyRelease(unsigned char keyCode);
    KeyEvent PopKey();

    void PushChar(wchar_t ch);
    wchar_t PopChar();

public:
    bool AutoRepeatKeys() const;
    void SetAutoRepeatKeys(bool autoRepeatKeys);

    bool AutoRepeatChars() const;
    void SetAutoRepeatChars(bool autoRepeatChars);

    bool KeyPressed(unsigned char keyCode) const;

    bool KeyQueueEmpty() const;
    bool CharQueueEmpty() const;

private:
    bool mAutoRepeatKeys = false;
    bool mAutoRepeatChars = false;
    bool mKeyPressed[UCHAR_MAX + 1] = {};
    std::queue<KeyEvent> mKeyQueue;
    std::queue<wchar_t> mCharQueue;
};
