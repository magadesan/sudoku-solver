#pragma once

#include <array>
#include <cstdint>

class State
{
private:
    using u8 = std::uint8_t;
    using u16 = std::uint16_t;

    std::array<u8, 92> mBytes;

public:
    State()
    {
        // Set all flags to true
        for (auto& b : mBytes)
        {
            b = 255;
        }
    }

    u16 GetCellState(u16 cellNo)
    {
        u16 skipBits = cellNo * u16(9);
        u8 skipBytes = skipBits / u16(8);

        u16 state = (mBytes[skipBytes] << u16(8)) + mBytes[skipBytes + u8(1)];

        u8 shiftBits = u16(7) - (skipBits - skipBytes * u16(8));

        state = (state & (u16(511) << shiftBits)) >> shiftBits;

        return state;
    }

    void SetCellState(u16 cellNo, u16 state)
    {
        u16 skipBits = cellNo * u16(9);
        u8 skipBytes = skipBits / u16(8);

        u8& byte1 = mBytes[skipBytes];
        u8& byte2 = mBytes[skipBytes + u8(1)];

        u8 shiftBits = u16(7) - (skipBits - skipBytes * u16(8));
        state = state << shiftBits;

        u16 mask = ~(u16(511) << shiftBits);

        byte1 = byte1 & (mask >> u16(8));
        byte1 = byte1 | (state >> u16(8));

        byte2 = byte2 & ((mask << u16(8)) >> u16(8));
        byte2 = byte2 | ((state << u16(8)) >> u16(8));
    }
};
