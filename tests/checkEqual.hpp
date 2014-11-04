#include <iostream>

template <typename U, typename V>
void CheckEqual(int line, const U& a, const V& b)
{
    if (a != b)
    {
        std::cout << line << ": " << a << " != " << b << std::endl;
    }
}