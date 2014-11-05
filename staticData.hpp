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
    static const std::array<std::array<uint8_t, 9>, 27> groups = CreateGroups();
    return groups;
}

std::uint32_t GetCellGroups(uint8_t cellNo)
{
    return (
        (1u << (cellNo / 9u)) |
        (1u << (9u + cellNo % 9u)) |
        (1u << (18u + 3u * (cellNo / 27u) + (cellNo % 9u) / 3u)));
}
