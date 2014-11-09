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
    using u64 = std::uint64_t;

    State mState;
    std::vector<State>* mCurrSolutions = nullptr;
    CallbackQueue mDirtyCells;
    bool mContradiction = false;

    void ProcessCell(u8 cellNo)
    {
        u16 cell = mState.GetCell(cellNo);

        for (u8 neighbourNo : GetCellNeighbours(cellNo))
        {
            u16 neighbour = mState.GetCell(neighbourNo);

            if ((neighbour & cell) != 0u)
            {
                neighbour &= ~cell;
                if ((neighbour & (neighbour - u16(1))) == 0u) // TODO: try getting rid of all instances of u8(1) etc.
                {
                    if (neighbour == 0u)
                    {
                        mContradiction = true;
                        return;
                    }

                    mDirtyCells.Push(neighbourNo);
                }
            }

            mState.SetCell(neighbourNo, neighbour);
        }
    }

    std::string GetDirtyCellsInBinary() const
    {
        std::string result(81, '0');
        auto dirtyCells = mDirtyCells;

        while (true)
        {
            u8 cellNo = dirtyCells.Pop();

            if (cellNo == u8(255))
                break;

            result[cellNo] = '1';
        }

        return result;
    }

    void ProcessCurrState()
    {
        while (!mDirtyCells.Empty())
        {
            u8 cellNo = mDirtyCells.Pop();
            ProcessCell(cellNo);

            if (mContradiction)
            {
                break;
            }
        }
    }

    void MarkFinalCellsDirty()
    {
        for (u8 cellNo = 0u; cellNo != 81u; ++cellNo)
        {
            u16 cell = mState.GetCell(cellNo);
            if ((cell & (cell - u16(1))) == 0u)
            {
                mDirtyCells.Push(cellNo);
            }
        }
    }

    void Guess(u8 cellNo, u16 cell)
    {
        mState.SetCell(cellNo, cell);
        mDirtyCells.Push(cellNo);
    }

    void SolveImpl()
    {
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

        u8 guessCellNo = mState.PickNonFinalisedCell();

        if (guessCellNo == u8(255))
        {
            mCurrSolutions->push_back(mState);
            return;
        }

        u16 cell = mState.GetCell(guessCellNo);
        u16 newCell = u16(1) << u16(8);

        State backupState = mState;

        while (newCell > u16(0))
        {
            if ((newCell & cell) != u16(0))
            {
                Guess(guessCellNo, newCell);
                SolveImpl();
                mState = backupState;
                mDirtyCells.Clear();
            }

            newCell >>= u16(1);
        }
    }

public:
    State ShallowSolve(const State& state)
    {
        mState = state;

        MarkFinalCellsDirty();
        ProcessCurrState();

        return mState;
    }

    std::vector<State> Solve(const State& state)
    {
        std::vector<State> solutions;
        mCurrSolutions = &solutions;

        mState = state;

        MarkFinalCellsDirty();
        SolveImpl();

        mCurrSolutions = nullptr;

        return solutions;
    }
};
