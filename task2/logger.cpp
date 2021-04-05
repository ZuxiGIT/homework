#include "logger.hpp"
#include <sstream>
#include <iostream>
#include <cassert>


std::ofstream Logger::m_output = {};
std::string LogObj::lastObj = {};
std::string LogFunc::current_fucntion = "";
int LogFunc::function_counter = 0;
bool Logger::m_created = false;
int Logger::m_shift  = 0;

Logger::Logger(const std::string& filename)
{
	_ERROR("LOGGER(std::string)\n")
    assert(!m_created && "ERROR::FILE_IS_ALREADY_OPENED\n" );
    
    m_output.open(filename, std::ios_base::out | std::ios_base::trunc);
    
    if(!m_output.is_open())
    {
        _ERROR("ERROR::CANNOT_OPEN_FILE\n")
        exit(1);
    }

    m_created = true;
    m_output << "digraph test{\n";
    
    m_shift++;
    shift();
    m_output << "rankdir=TB;\n";
}

Logger::Logger()
{
    //assert(m_created && "Output was not set");
    _ERROR("LOGGER()\n")
}

void Logger::shift()
{
    for(int i = 0; i < m_shift; ++i)
        m_output << "\t";
}

void Logger::close()
{
	m_output << "}";
    _ERROR("CLOSING FILE\n")
    m_output.close();
}

Logger::~Logger()
{
	_ERROR("~Logger()\n")
}



LogFunc::LogFunc(const std::string& funcname)
{
	function_counter++;
	_ERROR("LOGFUNC()\n")
	
	if(m_output.is_open())
		_ERROR("-----------------Everything os OK\n")
    
	shift();
    m_output << "subgraph cluster_" << _CHAR(function_counter) << "{\n"; 
    
    m_shift++;

    shift();
    m_output << "color=lightgrey;\n";
    shift();
    m_output << "style=filled;\n";
    shift();
    m_output << "label= \""<< funcname << "\";\n";

}

LogFunc::~LogFunc()
{
	m_shift--;
	shift();
	m_output << "}\n";

    _ERROR("~LOGFUNC()\n");
}

LogObj::LogObj(Obj2log& obj)
{
	_ERROR("LogObj()\n")	
	std::string Node = obj.createGraphNode();
	
	if(!Node.empty())
	{
		shift();
		m_output << Node;
	}

	std::string edge = obj.createHistoryEdge();
	
	if(!edge.empty())
	{
		shift();
		m_output << edge;
	}

}

LogObj::~LogObj()
{
	_ERROR("~LogObj()\n")
}