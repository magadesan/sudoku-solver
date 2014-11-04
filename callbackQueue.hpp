#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

class CallbackQueue
{
private:
    using u8 = std::uint8_t;
    static const u8 mCapacity = 27u;
    static const u8 mQueueArraySize = mCapacity + 1u;
    
    std::array<u8, mQueueArraySize> mElements;
    std::array<bool, mCapacity> mElementFlags;
    u8 mHead = 0u;
    u8 mTail = 0u;

public:
    CallbackQueue()
    {
        mElementFlags.fill(false);
    }
    
    bool Empty() const { return (mHead == mTail); }
    
    u8 Size() const { return (mTail + mQueueArraySize - mHead) % mQueueArraySize; }
    
    void Push(u8 x)
    {
        assert(x < mCapacity);
        
        if (!mElementFlags[x])
        {
            mElements[mTail] = x;
            mElementFlags[x] = true;
            mTail = (mTail + u8(1)) % mQueueArraySize;
        }
        
        AssertStateConsistent();
    }
    
#ifndef NDEBUG
    void AssertStateConsistent()
    {
        assert(CountFlags() == Size());
        
        if (Empty())
        {
            assert(CountFlags() == u8(0));
            return;
        }
        
        std::uint8_t curr = mHead;
        
        do
        {
            assert(mElementFlags[mElements[curr]]);
            
            curr = (curr + u8(1)) % mQueueArraySize;
        }
        while (curr != mTail);
    }
    
    u8 CountFlags()
    {
        u8 count = u8(0);
        
        for (bool x : mElementFlags)
        {
            if (x)
                ++count;
        }
        
        return count;
    }
#else
    void AssertStateConsistent() { }
#endif
    
    u8 PopFront()
    {
        if (Empty())
            return u8(255);
        
        u8 result = mElements[mHead];
        assert(mElementFlags[result] == true);
        mElementFlags[result] = false;
        mHead = (mHead + u8(1)) % mQueueArraySize;
        
        return result;
    }
    
    u8 PopBack()
    {
        if (Empty())
            return u8(255);
        
        mTail = (mTail + mQueueArraySize - u8(1)) % mQueueArraySize;
        u8 result = mElements[mTail];
        assert(mElementFlags[result] = true);
        mElementFlags[result] = false;
        
        return result;
    }
};
