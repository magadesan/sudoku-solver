#include <iostream>

const int foo = ([]()
{
    {
        int i = 3;
    }

    {
        ++i;
    }

    return 0;
})();

int main()
{
    std::cout << foo << std::endl;

    return 0;
}