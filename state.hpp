#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>

class State
{
private:
    using u8 = std::uint8_t;
    using u16 = std::uint16_t;

    std::array<u16, 81> mCells;

public:
    State()
    {
        // Set all flags to true
        for (auto& c : mCells)
        {
            c = 511;
        }
    }

    u16 GetCell(u16 cellNo) const
    {
        return mCells[cellNo];
    }

    void SetCell(u16 cellNo, u16 state)
    {
        mCells[cellNo] = state;
    }

    u8 PickNonFinalisedCell()
    {
        u8 bestCellNo = 255;
        u8 bestPossibilities = 255;

        for (u8 i = 0u; i != 81u; ++i)
        {
            u16 cell = GetCell(i);
            u8 possibilities = NumPossibilities(cell);

            // Shortcut: can't do better than 2
            if (possibilities == u8(2))
            {
                return i;
            }

            if (possibilities > u8(1) && possibilities < bestPossibilities)
            {
                bestCellNo = i;
                bestPossibilities = possibilities;
            }
        }

        return bestCellNo;
    }

    // TODO: there are other ways to do this, investigate their #performance
    static u8 NumPossibilities(u16 cell)
    {
        return __builtin_popcount(cell);
    }

    static u8 CellToNum(u16 cell)
    {
        if (cell == 0 || (cell & (cell - u16(1))) != 0)
        {
            return 0;
        }

        u8 n = 9;
        u16 nn = 1;

        while (nn != cell)
        {
            nn <<= 1;
            --n;
        }

        return n;
    }

    static u16 NumToCell(u8 n)
    {
        if (u8(1) <= n && n <= u8(9))
        {
            --n;
            return u16(1) << (u8(8) - n);
        }

        assert(n == 0);

        return 511;
    }

    friend std::ostream& operator<<(std::ostream& os, const State& state)
    {
        for (u8 i = 0u; i != 81u; ++i)
        {
            os << int(State::CellToNum(state.GetCell(i)));
        }

        return os;
    }

    friend std::istream& operator>>(std::istream& is, State& state)
    {
        for (u8 i = 0u; i != 81u; ++i)
        {
            u8 n;
            is >> n;
            assert(u8('0') <= n && n <= u8('9'));
            state.SetCell(i, State::NumToCell(n - u8('0')));
        }

        return is;
    }
};
