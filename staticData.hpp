#pragma once

#include <array>
#include <cstdint>

std::array<std::array<uint8_t, 9>, 27> CreateGroups()
{
    std::array<std::array<uint8_t, 9>, 27> ret;

    // rows
    for (int i = 0; i != 9; i++)
    {
        for (int j = 0; j != 9; j++)
        {
            ret[i][j] = 9 * i + j;
        }
    }

    // columns
    for (int i = 9; i != 18; i++)
    {
        for (int j = 0; j != 9; j++)
        {
            ret[i][j] = (i - 9) + 9 * j;
        }
    }

    // blocks
    for (int i = 18; i != 27; i++)
    {
        for (int j = 0; j != 9; j++)
        {
            int ni = i - 18;
            ret[i][j] = 27 * (ni / 3) + 3 * (ni % 3) + 9 * (j / 3) + (j % 3);
        }
    }

    return ret;
}

const std::array<std::array<uint8_t, 9>, 27>& GetGroups()
{
    static std::array<std::array<uint8_t, 9>, 27> groups = CreateGroups();
    return groups;
}

std::array<uint8_t, 3> GetCellGroup(uint8_t cellNo)
{
    return std::array<uint8_t, 3>{{
        uint8_t(cellNo / 9),
        uint8_t(9 + cellNo % 9),
        uint8_t(18 + 3 * (cellNo / 27) + (cellNo % 9) / 3)
    }};
}