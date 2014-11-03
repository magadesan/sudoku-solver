#include <cassert>
#include <iostream>

#include "../state.hpp"

void AssertEqual(int line, int a, int b)
{
    if (a != b)
    {
        std::cout << line << ": " << a << " != " << b << std::endl;
    }
}

int main()
{
    State s;

    for (uint8_t cellNo = 0u; cellNo < 81u; ++cellNo)
    {
        AssertEqual(__LINE__, s.GetCellState(cellNo), 511);
        s.SetCellState(cellNo, 137);
    }

    for (uint8_t cellNo = 0u; cellNo < 81u; ++cellNo)
    {
        AssertEqual(__LINE__, s.GetCellState(cellNo), 137);
    }

    return 0;
}
