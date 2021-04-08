#include "Number.hpp"
//#include "log.hpp"
#include <sstream>
#include <stdio.h>
#include <stdlib.h>


int Number::num_of_objects = 0;
int Number::max_size = 0;
int num_of_temporary = -1;
// int Number::m_compare_number = 0;
// int Number::m_copy_number = 0;




template <typename T>
std::string pointer2str(T value)
{
    std::ostringstream str;
    str << std::hex << value;
    //_ERROR((str.str() + "\n").c_str())
    return str.str();
}


std::string prop::getGraphNode(int value) const
{
    std::string res = "";

    res += "node_"   + _CHAR(overall_number)           + \
    " [shape=record, label= \"{"                       + \
    " <name> name: "        + std::string(name)        + \
    " | <value> value: "     + _CHAR(value)            + \
    " | <number> number: "    + _CHAR(overall_number)  + \
    " | <address> address: "   + pointer2str(address)  + \
    " | <constr> constr : "   + constructor            + \
    "}\"];\n";
    
    /* 
    res += "node_" + _CHAR(origin_node)       + \
    "->node_" + _CHAR(overall_number);
    */
    return res;
}

std::string prop::getGraphNodeInfo(int value, std::string add_name) const
{
    std::string res = "";

    res += \
    " { <name_" + add_name + "> name: " + std::string(name)      + \
    " | <value> value: "     + _CHAR(value)                      + \
    " | <number> number: "   + _CHAR(overall_number)             + \
    " | <address> address: " + pointer2str(address)              + \
    " | <constr> constr : "  + constructor                       + \
    "}";

    return res;
}
std::string prop::getGraphEdge() const
{
    std::string res = "";

    if(origin_node != 0)
        res += "node_" + _CHAR(origin_node) + "->"   +\
        "node_" + _CHAR(overall_number) + "[style= " +\
        "dashed];\n";

    return res;
}

int prop::getNumber() const
{
    return overall_number;
}



// void Number::printStat()
// {
//     fprintf(stderr,"Overall compare: %d\n copy:%d\n", m_compare_number, m_copy_number);
// }

Number operator+ (const Number& lhs, const Number& rhs)
{
    //Log log(__PRETTY_FUNCTION__);
    //log.function(lhs, rhs);
    //log.summary();


    return Number(lhs.val + rhs.val);
}
Number operator- (const Number& lhs, const Number& rhs)
{
    //Log log(__PRETTY_FUNCTION__);
    //log.function(lhs, rhs);
    //log.summary();
    
    return Number(lhs.val - rhs.val);
}
Number operator* (const Number& lhs, const Number& rhs)
{    
    //Log log(__PRETTY_FUNCTION__);
    //log.function(lhs, rhs);
    //log.summary();
    
    return Number(lhs.val * rhs.val);
}
Number operator/ (const Number& lhs, const Number& rhs)
{
    //Log log(__PRETTY_FUNCTION__);
    //log.function(lhs, rhs);
    //log.summary();
    
    return Number(lhs.val / rhs.val);
}

Number operator% (const Number& lhs, const Number& rhs)
{
    //Log log(__PRETTY_FUNCTION__);
    //log.function(lhs, rhs);
    //log.summary();
    
    return Number(lhs.val % rhs.val);
}

Number operator& (const Number& lhs, const Number& rhs)
{
    //Log log(__PRETTY_FUNCTION__);
    //log.function(lhs, rhs);
    //log.summary();
    
    return Number(lhs.val & rhs.val);
}

Number operator|(const Number& lhs, const Number& rhs)
{
    //Log log(__PRETTY_FUNCTION__);
    //log.function(lhs, rhs);
    //log.summary();
    
    return Number( lhs.val | rhs.val);
}

Number::~Number()
{
    num_of_objects--;
    //Log log(__PRETTY_FUNCTION__, RED);
    //log.printProp(*this);
    //log.summary();
}

Number::Number(prop ss)
: val(0),
  info(ss)
{
    info.constructor = __PRETTY_FUNCTION__;
    info.current_number = ++num_of_objects;
    info.overall_number = ++max_size;

    info.address = this;

    LogObj (*this);


    //Log log(__PRETTY_FUNCTION__, GREEN);
    //log.printProp(*this);
}

Number::Number(int num, prop ss)
: val(num), 
  info(ss)
{
    info.constructor = __PRETTY_FUNCTION__;
    info.current_number = ++num_of_objects;
    info.overall_number = ++max_size;

    info.address = this;

    LogObj (*this);

    //Log log(__PRETTY_FUNCTION__, GREEN);
    //log.printProp(*this);

}

Number::Number(double num, prop ss)
: val(static_cast<int>(num)),
  info(ss)
{
    info.constructor = __PRETTY_FUNCTION__;
    info.current_number = ++num_of_objects;
    info.overall_number = ++max_size;
    //Log log(__PRETTY_FUNCTION__, GREEN);
    //log.printProp(*this);
    
    info.address = this;
    
    LogObj (*this);
}

Number::Number(const char* str, prop ss)
: val(atoi(str)), 
  info(ss)
{
    info.constructor = __PRETTY_FUNCTION__;
    info.current_number = ++num_of_objects;
    info.overall_number = ++max_size;;
    //Log log(__PRETTY_FUNCTION__, GREEN);
    //log.printProp(*this);

    info.address = this;
    
    LogObj (*this);
}

Number::Number(const Number& rhs, prop ss)
: val(rhs.val), 
  info(ss)
{
    //Log log(__PRETTY_FUNCTION__, MAGENTA);
    info.constructor = __PRETTY_FUNCTION__;
    info.current_number = ++num_of_objects;
    info.overall_number = ++max_size;;

    info.address = this;

    LogObj (*this);
    //m_copy_number++;

    //log.copy(*this, rhs);
    //log.printProp(*this);
}

Number::Number(const Number&& rhs)
: val(rhs.val), 
  info({"THIEF", __FILE__ , -5, num_of_objects + 1})
{
    //Log log(__PRETTY_FUNCTION__, MAGENTA);
    info.constructor = __PRETTY_FUNCTION__;
    info.current_number = ++num_of_objects;
    info.overall_number = ++max_size;;

    info.address = this;
    
    LogObj (*this);
    //m_copy_number++;

    //log.copy(*this, rhs);
    //log.printProp(*this);
}