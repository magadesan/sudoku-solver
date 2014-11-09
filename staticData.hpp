#pragma once

#include <array>
#include <bitset>
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

std::array<std::array<std::uint8_t, 20>, 81> CreateCellNeighbours()
{
    using u8 = std::uint8_t;
    std::array<std::array<u8, 20>, 81> neighbours;
    const auto& groups = GetGroups();

    for (u8 i = 0u; i != 81u; ++i)
    {
        std::bitset<81> set;

        for (u8 groupNo : (u8[]){u8(i / 9u), u8(9u + i % 9u), u8(18u + 3u * (i / 27u) + (i % 9u) / 3u)})
        {
            for (auto neighbour : groups[groupNo])
            {
                set.set(neighbour);
            }
        }

        auto& nn = neighbours[i];

        u8 k = 0u;
        for (u8 j = 0u; j != 81u; j++)
        {
            if (set.test(j) && i != j)
            {
                nn[k] = j;
                ++k;
            }
        }
    }

    return neighbours;
}

const std::array<std::uint8_t, 20>& GetCellNeighbours(std::uint8_t cellNo)
{
    static const std::array<std::array<std::uint8_t, 20>, 81> cellNeighbours = CreateCellNeighbours();
    return cellNeighbours[cellNo];
}
