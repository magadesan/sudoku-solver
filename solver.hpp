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

    void MarkCellGroups(u8 cellNo)
    {
        mDirtyGroups.PushFlags(GetCellGroups(cellNo));
    }

    void ProcessGroup(u8 groupNo)
    {
        auto& group = GetGroups()[groupNo];

        u16 mask = 511;

        for (auto cellNo : group)
        {
            u16 cell = mCurrState->GetCell(cellNo);
            u16 cellOld = cell;
            cell &= mask;

            mCurrState->SetCell(cellNo, cell);

            if ((cell & (cell - u16(1))) == 0)
            {
                if (cell == 0)
                {
                    mContradiction = true;
                    break;
                }

                mask &= ~cell;

                if (cell != cellOld)
                {
                    MarkCellGroups(cellNo);
                }
            }
        }

        mask = 511;

        for (auto it = group.rbegin(); it != group.rend(); ++it)
        {
            auto cellNo = *it;
            u16 cell = mCurrState->GetCell(cellNo);
            u16 cellOld = cell;
            cell &= mask;
            mCurrState->SetCell(cellNo, cell);

            if ((cell & (cell - u16(1))) == 0)
            {
                if (cell == 0)
                {
                    mContradiction = true;
                    break;
                }

                mask &= ~cell;

                if (cell != cellOld)
                {
                    MarkCellGroups(cellNo);
                }
            }
        }

        mDirtyGroups.PopElement(groupNo);
    }

    void ProcessCurrState()
    {
        while (!mDirtyGroups.Empty())
        {
            ProcessGroup(mDirtyGroups.Pop());

            if (mContradiction)
            {
                break;
            }
        }
    }

    void MarkAllGroupsDirty()
    {
        mDirtyGroups.PushFlags((~0u) - ((~0u) << 27u));
        assert(mDirtyGroups.Size() == 27u);
    }

    void Guess(u8 cellNo, u16 cell)
    {
        mCurrState->SetCell(cellNo, cell);
        MarkCellGroups(cellNo);
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
