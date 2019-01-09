#pragma once

template <class T, class E>
class Expected final
{
public:
    // ReSharper disable CppNonExplicitConvertingConstructor

    Expected(const T& t);
    Expected(T&& t);

    Expected(const E& e);
    Expected(E&& e);

    // ReSharper restore CppNonExplicitConvertingConstructor

    Expected(const Expected& expected);
    Expected(Expected&& expected) noexcept;

    Expected& operator=(const Expected& expected);
    Expected& operator=(Expected&& expected) noexcept;

    ~Expected();

public:
    bool Valid() const;

    T& Value();
    const T& Value() const;

    const E& Error() const;

private:
    bool mValid;

    union
    {
        T mT;
        E mE;
    };
};

#include "Expected.inl"
