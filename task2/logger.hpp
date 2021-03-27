#pragma once
#include <stdio.h>
#include <sstream>
#include <fstream>

class Logger
{
    bool m_created = false;
    int m_shift  = 0;
protected:
    std::ofstream m_output = {};
public:
    void shift(); 
    Logger();
    Logger(std::string&& filename);
    ~Logger();
};


class LogFunc : Logger
{
     std::string m_lastFunc = {};
     std::string m_currFunc = {};
public:
    LogFunc();

    LogFunc(const LogFunc&) = delete;
    LogFunc& operator=(const LogFunc&) = delete;

    ~LogFunc();
};

class LogObj : Logger
{

public:
    LogObj();

    LogObj(const LogObj&) = delete;
    LogObj& operator=(const LogObj&) = delete;

    ~LogObj();

};