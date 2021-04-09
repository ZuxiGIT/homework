#include "Number.hpp"
#include "logger.hpp"

inline void ant(int a)
{
    LogFunc lgf(__PRETTY_FUNCTION__);
    if(a > 10)
        ant(a - 5);
    NUMBER_(ant_var, a);
    return;
}

inline void test_func(Number a, Number b)
{
    LogFunc lgf(__PRETTY_FUNCTION__);
    LogObj(a, b);
    NUMBER_(BEFORE_test_func_var, 43);
    ant(13);
    NUMBER_(AFTER_test_func_var, 34);
    return;
}

int main()
{
    Logger mylog{"dot_test.dot"};
    LogFunc lgf(__PRETTY_FUNCTION__);

    NUMBER_(a);
    NUMBER_(b, 10);

    NUMBER_(c, a + b);
    b = a - c;

    test_func(a, b);
    NUMBER_(ddddddd, a);
}