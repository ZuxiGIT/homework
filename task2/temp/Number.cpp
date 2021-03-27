#include "Number.hpp"
#include "Log.hpp"
#include <stdio.h>
#include <stdlib.h>

int Number::num_of_objects = 0;
int Number::max_size = 0;
int num_of_temporary = -1;
int Number::m_compare_number = 0;
int Number::m_copy_number = 0;

void Number::printStat()
{
    fprintf(stderr,"Overall compare: %d\n copy:%d\n", m_compare_number, m_copy_number);
}

Number operator+ (const Number& lhs, const Number& rhs)
{
    Log log(__PRETTY_FUNCTION__);
    log.function(lhs, rhs);
    log.summary();

    return Number(lhs.val + rhs.val);
}
Number operator- (const Number& lhs, const Number& rhs)
{
    Log log(__PRETTY_FUNCTION__);
    log.function(lhs, rhs);
    log.summary();
    
    return Number(lhs.val - rhs.val);
}
Number operator* (const Number& lhs, const Number& rhs)
{    
    Log log(__PRETTY_FUNCTION__);
    log.function(lhs, rhs);
    log.summary();
    
    return Number(lhs.val * rhs.val);
}
Number operator/ (const Number& lhs, const Number& rhs)
{
    Log log(__PRETTY_FUNCTION__);
    log.function(lhs, rhs);
    log.summary();
    
    return Number(lhs.val / rhs.val);
}

Number operator% (const Number& lhs, const Number& rhs)
{
    Log log(__PRETTY_FUNCTION__);
    log.function(lhs, rhs);
    log.summary();
    
    return Number(lhs.val % rhs.val);
}

Number operator& (const Number& lhs, const Number& rhs)
{
    Log log(__PRETTY_FUNCTION__);
    log.function(lhs, rhs);
    log.summary();
    
    return Number(lhs.val & rhs.val);
}

Number operator|(const Number& lhs, const Number& rhs)
{
    Log log(__PRETTY_FUNCTION__);
    log.function(lhs, rhs);
    log.summary();
    
    return Number( lhs.val | rhs.val);
}

Number::~Number()
{
    num_of_objects--;
    Log log(__PRETTY_FUNCTION__, RED);
    log.printProp(*this);
    log.summary();
}

Number::Number(prop ss)
: val(0),
  info(ss)
{
    info.constructor = __PRETTY_FUNCTION__;
    info.number = ++num_of_objects;
    max_size++;

    Log log(__PRETTY_FUNCTION__, GREEN);
    log.printProp(*this);
}

Number::Number(int num, prop ss)
: val(num), 
  info(ss)
{
    info.constructor = __PRETTY_FUNCTION__;
    info.number = ++num_of_objects;
    max_size++;
    Log log(__PRETTY_FUNCTION__, GREEN);
    log.printProp(*this);

}

Number::Number(double num, prop ss)
: val(static_cast<int>(num)),
  info(ss)
{
    info.constructor = __PRETTY_FUNCTION__;
    info.number = ++num_of_objects;
    max_size++;
    Log log(__PRETTY_FUNCTION__, GREEN);
    log.printProp(*this);

}

Number::Number(const char* str, prop ss)
: val(atoi(str)), 
  info(ss)
{
    info.constructor = __PRETTY_FUNCTION__;
    info.number = ++num_of_objects;
    max_size++;
    Log log(__PRETTY_FUNCTION__, GREEN);
    log.printProp(*this);

}

Number::Number(const Number& rhs, prop ss)
: val(rhs.val), 
  info(ss)
{
    Log log(__PRETTY_FUNCTION__, MAGENTA);
    info.constructor = __PRETTY_FUNCTION__;
    info.number = ++num_of_objects;
    max_size++;

    m_copy_number++;

    log.copy(*this, rhs);
    log.printProp(*this);
}

Number::Number(const Number&& rhs)
: val(rhs.val), 
  info({"THIEF", __FILE__ , -5, num_of_objects + 1})
{
    Log log(__PRETTY_FUNCTION__, MAGENTA);
    info.constructor = __PRETTY_FUNCTION__;
    info.number = ++num_of_objects;
    max_size++;

    m_copy_number++;

    log.copy(*this, rhs);
    log.printProp(*this);
}