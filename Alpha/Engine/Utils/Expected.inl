#pragma once

#include "Debug.h"

template <typename T, typename E>
Expected<T, E>::Expected(const T& t):
    mValid{true},
    mT{t}
{
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
}

template <typename T, typename E>
Expected<T, E>::Expected(T&& t):
    mValid{true},
    mT{std::move(t)}
{
    static_assert(std::is_move_constructible_v<T>);
    static_assert(std::is_move_assignable_v<T>);
}

template <typename T, typename E>
Expected<T, E>::Expected(const E& e):
    mValid{false},
    mE{e}
{
    static_assert(std::is_copy_constructible_v<E>);
    static_assert(std::is_copy_assignable_v<E>);
}

template <typename T, typename E>
Expected<T, E>::Expected(E&& e):
    mValid{false},
    mE{std::move(e)}
{
    static_assert(std::is_move_constructible_v<E>);
    static_assert(std::is_move_assignable_v<E>);
}

// ReSharper disable CppNonReclaimedResourceAcquisition

template <typename T, typename E>
Expected<T, E>::Expected(const Expected& expected):
    mValid{expected.mValid}
{
    if (mValid)
    {
        new(&mT) T{expected.mT};
    }
    else
    {
        new(&mE) E{expected.mE};
    }
}

template <typename T, typename E>
Expected<T, E>::Expected(Expected&& expected)
noexcept:
    mValid
    {expected.mValid}
{
    if (mValid)
    {
        new(&mT) T{std::move(expected.mT)};
    }
    else
    {
        new(&mE) E{std::move(expected.mE)};
    }
}

template <typename T, typename E>
Expected<T, E>& Expected<T, E>::operator=(const Expected& expected)
{
    mValid = expected.mValid;

    if (mValid)
    {
        new(&mT) T{expected.mT};
    }
    else
    {
        new(&mE) E{expected.mE};
    }

    return *this;
}

template <typename T, typename E>
Expected<T, E>& Expected<T, E>::operator=(Expected&& expected)
noexcept
{
    mValid = expected.mValid;

    if (mValid)
    {
        new(&mT) T{std::move(expected.mT)};
    }
    else
    {
        new(&mE) E{std::move(expected.mE)};
    }

    return *this;
}

// ReSharper restore CppNonReclaimedResourceAcquisition

template <typename T, typename E>
Expected<T, E>::~Expected()
{
    if (mValid)
    {
        mT.~T();
    }
    else
    {
        mE.~E();
    }
}

template <typename T, typename E>
bool Expected<T, E>::Valid() const
{
    return mValid;
}

template <typename T, typename E>
T& Expected<T, E>::Value()
{
    ASSERT(mValid);

    return mT;
}

template <typename T, typename E>
const T& Expected<T, E>::Value() const
{
    ASSERT(mValid);

    return mT;
}

template <typename T, typename E>
const E& Expected<T, E>::Error() const
{
    ASSERT(!mValid);

    return mE;
}
