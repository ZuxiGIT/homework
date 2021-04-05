#pragma once
#include <stdio.h>
#include <string>
#include <fstream>

#define _ERROR(_str) fprintf(stderr, _str);
#define _CHAR(x)  std::to_string(x)


struct Obj2log
{
	virtual std::string createGraphNode() = 0;//  { return ""; }
	virtual std::string createHistoryEdge() = 0;//{ return ""; }
};

class Logger
{

    static bool m_created;
protected:
    static int m_shift;
    static std::ofstream m_output;
    void shift(); 
public:
    Logger();
    explicit Logger(const std::string& filename);
    void close();
    ~Logger();
};


class LogFunc : public Logger
{
    std::string m_last_funcction = "";
    static std::string current_function;
    static int function_counter;
public:
    LogFunc(const std::string& funcname);

    LogFunc(const LogFunc&) = delete;
    LogFunc& operator=(const LogFunc&) = delete;

    ~LogFunc();
};

class LogObj : public Logger
{

	static std::string lastObj;
public:
    LogObj(Obj2log& obj);

    LogObj(const LogObj&) = delete;
    LogObj& operator=(const LogObj&) = delete;

    ~LogObj();

};

