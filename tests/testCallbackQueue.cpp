#include <cassert>
#include <iostream>

#include "../callbackQueue.hpp"

void AssertEqual(int line, int a, int b)
{
    if (a != b)
    {
        std::cout << line << ": " << a << " != " << b << std::endl;
    }
}

int main()
{
    CallbackQueue q;

    q.Push(1);
    q.Push(5);
    q.Push(7);
    q.Push(5);

    AssertEqual(__LINE__, q.PopBack(), 7);
    AssertEqual(__LINE__, q.PopFront(), 1);
    AssertEqual(__LINE__, q.PopBack(), 5);

    return 0;
}