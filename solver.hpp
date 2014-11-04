#pragma once

#include <cassert>
#include <cstdint>
#include <sstream>

#include "callbackQueue.hpp"
#include "state.hpp"
#include "staticData.hpp"

class Solver
{
private:
    using u8 = std::uint8_t;
    using u16 = std::uint16_t;

    State mState;
    CallbackQueue mDirtyGroups;
    bool mContradiction = false;

    void MarkCellDirty(u8 cellNo)
    {
        for (auto groupNo : GetCellGroups(cellNo))
        {
            mDirtyGroups.Push(groupNo);
        }
    }

    void ProcessFinalisedCell(u8 cellNo, u16 cell)
    {
        u16 mask = ~cell;

        for (auto groupNo : GetCellGroups(cellNo))
        {
            auto& group = GetGroups()[groupNo];

            for (auto subcellNo : group)
            {
                if (subcellNo == cellNo)
                    continue;

                u16 subcell = mState.GetCell(subcellNo);
                u16 newSubcell = (subcell & mask);

                if (subcell != newSubcell) // TODO: only mark dirty if value finalised? #performance
                {
                    for (auto subgroupNo : GetCellGroups(subcellNo))
                    {
                        mDirtyGroups.Push(subgroupNo);
                    }
                }

                mState.SetCell(subcellNo, newSubcell);
            }
        }
    }

    void ProcessGroup(u8 groupNo)
    {
        auto& group = GetGroups()[groupNo];

        for (auto cellNo : group)
        {
            u16 cell = mState.GetCell(cellNo);

            if ((cell & (cell - u16(1))) == 0)
            {
                if (cell == 0)
                {
                    mContradiction = true;
                    break;
                }

                ProcessFinalisedCell(cellNo, cell);
            }
        }
    }

public:
    Solver(State state)
    :
        mState(std::move(state))
    {
        for (u8 i = 0u; i != 27u; ++i)
        {
            mDirtyGroups.Push(i);
        }
    }

    const State& GetState() { return mState; }

    void Solve()
    {
        while (!mDirtyGroups.Empty())
        {
            ProcessGroup(mDirtyGroups.PopFront());

            if (mContradiction)
            {
                break;
            }
        }
    }
};
