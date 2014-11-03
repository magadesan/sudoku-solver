#include <iostream>

#include "../staticData.hpp"

int main()
{
    auto groups = CreateGroups();
    std::array<std::array<uint8_t, 3>, 81> cellGroups = {};

    int groupNo = 0;
    for (auto& group : groups)
    {
        for (auto& cellNo : group)
        {
            std::cout << int(cellNo) << ' ';
            
            for (int i = 0; i != 3; i++)
            {
                if (cellGroups[cellNo][i] == 0u)
                {
                    cellGroups[cellNo][i] = groupNo;
                    break;
                }
            }
        }

        ++groupNo;

        std::cout << std::endl;
    }

    for (uint8_t cellNo = 0u; cellNo < 81u; ++cellNo)
    {
        std::cout << "Cell " << int(cellNo) << ": ";
        
        for (int i = 0; i != 3; i++)
        {
            std::cout << int(GetCellGroup(cellNo)[i]) << ' ';
        }

        std::cout << std::endl;
    }

    return 0;
}