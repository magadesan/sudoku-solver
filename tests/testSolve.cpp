#include <iostream>
#include <sstream>

#include "../solver.hpp"

#include "checkEqual.hpp"

std::string StringSolve(const std::string& sudoku)
{
    std::istringstream iss(sudoku);
    State s;
    iss >> s;

    Solver solver;
    std::vector<State> solutions = solver.Solve(s);

    std::ostringstream oss;

    bool first = true;
    for (auto& solution : solutions)
    {
        if (!first)
            oss << ",";

        oss << solution;

        first = false;
    }

    return oss.str();
}

int main()
{
    std::array<std::array<std::string, 2>, 3> solverPairs = {{
        {{
            "092030071005090836340107200000304725037029100284700003109240060800573900470000582",
            "692835471715492836348167259961384725537629148284751693159248367826573914473916582"
        }},
        {{
            "000000010400000000020000000000050407008000300001090000300400200050100000000806000",
            "693784512487512936125963874932651487568247391741398625319475268856129743274836159"
        }},
        {{
            "100007090030020008009600500005300900010080002600004000300000010040000007007000300",
            "162857493534129678789643521475312986913586742628794135356478219241935867897261354"
        }}
    }};

    for (auto& pair : solverPairs)
    {
        CheckEqual(__LINE__, StringSolve(pair[0]), pair[1]);
    }

    return 0;
}
