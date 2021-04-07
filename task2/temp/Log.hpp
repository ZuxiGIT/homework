#pragma once

#include <wincon.h>

#define BLACK           0
#define BLUE            FOREGROUND_BLUE
#define GREEN           FOREGROUND_GREEN
#define CYAN            BLUE | GREEN
#define RED             FOREGROUND_RED
#define MAGENTA         RED | BLUE
#define DARKYELLOW      RED | GREEN
#define LIGHTGRAY       RED | GREEN | BLUE
#define DARKGRAY        FOREGROUND_INTENSITY
#define LIGHTBLUE       FOREGROUND_INTENSITY | BLUE
#define LIGHTGREEN      FOREGROUND_INTENSITY | GREEN
#define LIGHTCYAN       FOREGROUND_INTENSITY | CYAN
#define LIGHTRED        FOREGROUND_INTENSITY | RED
#define LIGHTMAGENTA    FOREGROUND_INTENSITY | MAGENTA
#define YELLOW          FOREGROUND_INTENSITY | DARKYELLOW
#define WHITE           FOREGROUND_INTENSITY | RED | GREEN | BLUE


typedef unsigned char COL;


class Number;

class Log 
{
    
    static int m_shift;

    void shift();
    void call(const char* function, COL background = CYAN);
    
    public:
    Log(const char* function, COL background = CYAN);
    ~Log();
    
    void copy(const Number& lhs, const Number& rhs);
    void function(const Number& lhs, const Number& rhs);
    void printProp(const Number& ss);
    void summary();
};

COL SetConsoleColour(COL foreground, COL background = BLACK);
COL GetConsoleColour();