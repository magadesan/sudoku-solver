#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

class CallbackQueue
{
private:
    using u8 = std::uint8_t;
    using u64 = std::uint64_t;
    std::array<u64, 2> mFlags = {{0u, 0u}};

public:
    bool Empty() const { return mFlags[0] == 0u && mFlags[1] == 0u; }
    
    u8 Size() const { return __builtin_popcountll(mFlags[0]) + __builtin_popcountll(mFlags[1]); }
    
    void Push(u8 x)
    {
        assert(x < 81u);
        bool first = x < 64u;
        u64 bit = 1u << (x % 64u);
        mFlags[0] |= first * bit;
        mFlags[1] |= !first * bit;
    }

    void PushFlags(std::array<u64, 2> flags) { mFlags[0] |= flags[0]; mFlags[1] |= flags[1]; }
    void PushFlags(u64 flags) { mFlags[0] |= flags; } // TODO: remove
    
    u8 Pop()
    {
        if (mFlags[0] != 0u)
        {
            u8 ret = __builtin_ctzll(mFlags[0]);
            mFlags[0] &= ~(1u << ret);
            return u8(ret);
        }
        else if (mFlags[1] != 0u)
        {
            u8 ret = __builtin_ctzll(mFlags[1]);
            mFlags[1] &= ~(1u << ret);
            return u8(64u + ret);
        }

        return u8(255);
    }

    void PopElement(u8 element)
    {
        if (element < 64u)
        {
            mFlags[0] &= ~(1u << element);
        }
        else
        {
            element -= 64u;
            mFlags[1] &= ~(1u << element);
        }
    }
};
