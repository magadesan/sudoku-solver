#pragma once

#include <cassert>
#include <cstdint>
#include <functional>
#include <sstream>
#include <vector>

#include "callbackQueue.hpp"
#include "state.hpp"
#include "staticData.hpp"

class Solver
{
private:
    using u8 = std::uint8_t;
    using u16 = std::uint16_t;

    State* mCurrState = nullptr;
    std::vector<State>* mCurrSolutions = nullptr;
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

                u16 subcell = mCurrState->GetCell(subcellNo);
                u16 newSubcell = (subcell & mask);

                if (subcell != newSubcell) // TODO: only mark dirty if value finalised? #performance
                {
                    for (auto subgroupNo : GetCellGroups(subcellNo))
                    {
                        mDirtyGroups.Push(subgroupNo);
                    }
                }

                mCurrState->SetCell(subcellNo, newSubcell);
            }
        }
    }

    void ProcessGroup(u8 groupNo)
    {
        auto& group = GetGroups()[groupNo];

        for (auto cellNo : group)
        {
            u16 cell = mCurrState->GetCell(cellNo);

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

    void ProcessCurrState()
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

    void MarkAllGroupsDirty()
    {
        for (u8 i = 0u; i != 27u; ++i)
        {
            mDirtyGroups.Push(i);
        }
    }

    void Guess(u8 cellNo, u16 cell)
    {
        mCurrState->SetCell(cellNo, cell);
        ProcessFinalisedCell(cellNo, cell);
    }

    void SolveImpl(const State& state)
    {
        State currState = state;
        mCurrState = &currState;

        ProcessCurrState();

        ProcessingExhausted();
    }

    void SolveImplWithGuess(const State& state, u8 cellNo, u16 cell)
    {
        State currState = state;
        mCurrState = &currState;

        Guess(cellNo, cell);
        ProcessCurrState();

        ProcessingExhausted();
    }

    void ProcessingExhausted()
    {
        if (mContradiction)
        {
            mContradiction = false;
            return;
        }

        // TODO: find non-finalised cell with fewest possibilities? #performance
        u8 guessCellNo = mCurrState->PickNonFinalisedCell();

        if (guessCellNo == u8(255))
        {
            mCurrSolutions->push_back(*mCurrState);
            return;
        }

        u16 cell = mCurrState->GetCell(guessCellNo);
        u16 newCell = u16(1) << u16(8);

        State* backupState = mCurrState;

        while (newCell > u16(0))
        {
            if ((newCell & cell) != u16(0))
            {
                SolveImplWithGuess(*backupState, guessCellNo, newCell);
            }

            newCell >>= u16(1);
        }

        mCurrState = backupState;
    }

public:
    State ShallowSolve(const State& state)
    {
        MarkAllGroupsDirty();

        State currState = state;
        mCurrState = &currState;

        ProcessCurrState();

        mCurrState = nullptr;

        return currState;
    }

    std::vector<State> Solve(const State& state)
    {
        std::vector<State> solutions;
        mCurrSolutions = &solutions;

        MarkAllGroupsDirty();
        SolveImpl(state);

        mCurrSolutions = nullptr;

        return solutions;
    }
};
