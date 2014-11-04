#include <iostream>

#include "solver.hpp"

int main()
{
    Solver solver;

    while (true)
    {
        State puzzle;
        std::cin >> puzzle;

        if (!std::cin)
        {
            break;
        }

        std::vector<State> solutions =  solver.Solve(puzzle);

        bool first = true;
        for (State& s : solutions)
        {
            if (!first)
                std::cout << ",";

            std::cout << s;
        }

        std::cout << std::endl;
    }

    return 0;
}