#pragma once

class NonCopyable
{

    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator= (const NonCopyable&) = delete;
protected:

    NonCopyable() {}
    virtual ~NonCopyable() {}
};