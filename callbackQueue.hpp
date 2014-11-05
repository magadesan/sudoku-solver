#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

class CallbackQueue
{
private:
    using u8 = std::uint8_t;
    using u32 = std::uint32_t;
    u32 mFlags = 0u;

public:
    bool Empty() const { return mFlags == 0u; }
    
    u8 Size() const { return __builtin_popcount(mFlags); }
    
    void Push(u8 x)
    {
        assert(x < 27u);
        mFlags |= 1u << x;
    }

    void PushFlags(u32 flags) { mFlags |= flags; }
    
    u8 Pop()
    {
        if (Empty())
            return u8(255);

        u8 ret = __builtin_ctz(mFlags);
        PopElement(ret);
        
        return ret;
    }

    void PopElement(u8 element)
    {
        mFlags &= ~(1u << element);
    }
};
