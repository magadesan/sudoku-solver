#include <cassert>
#include <iostream>
#include <sstream>

#include "../state.hpp"

template <typename U, typename V>
void AssertEqual(int line, const U& a, const V& b)
{
    if (a != b)
    {
        std::cout << line << ": " << a << " != " << b << std::endl;
    }
}

void TestGetAndSet()
{
    State s;

    for (uint8_t cellNo = 0u; cellNo < 81u; ++cellNo)
    {
        AssertEqual(__LINE__, s.GetCell(cellNo), 511);
        s.SetCell(cellNo, 137);
    }

    for (uint8_t cellNo = 0u; cellNo < 81u; ++cellNo)
    {
        AssertEqual(__LINE__, s.GetCell(cellNo), 137);
    }
}

void TestIo()
{
    State s;
    std::ostringstream oss;
    oss << s;
    
    AssertEqual(
        __LINE__,
        oss.str(),
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000");

    oss.str("");

    for (int i = 0; i != 81; ++i)
    {
        s.SetCell(i, State::NumToCell(i % 9 + 1));
    }

    oss << s;

    AssertEqual(
        __LINE__,
        oss.str(),
        "123456789123456789123456789123456789123456789123456789123456789123456789123456789");

    oss.str("");

    std::istringstream iss;
    iss.str("123123123456456456789789789123123123456456456789789789123123123456456456789789789");
    iss >> s;
    oss << s;

    AssertEqual(
        __LINE__,
        oss.str(),
        "123123123456456456789789789123123123456456456789789789123123123456456456789789789");

    iss.str("");
    oss.str("");
}

int main()
{
    TestGetAndSet();
    TestIo();

    return 0;
}
