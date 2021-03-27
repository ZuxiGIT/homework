#include <stdio.h>
#include <stdlib.h>
#include "Log.hpp"
#include "Number.hpp"



#define NUMBER_(var,...) Number var (__VA_ARGS__ __VA_OPT__(,) {#var , __FILE__, __LINE__})






#define LOCATE(var) {#var , __FILE__, __LINE__}
Number incr(Number);
Number sum(Number, Number);

Number incr(Number a)
{
    return Number( a + 1);
}

Number sum(Number a, Number b)
{
    return Number(a + b);
}


int comp(const void* a, const void* b)
{
    return (*(Number*)a - *(Number*) b).getval();
}

int main()
{
    {
        SetConsoleColour(LIGHTCYAN);
        fprintf(stderr, "------------begin of scope------------\n");
        SetConsoleColour(WHITE, BLACK);
        //Number a{5};
        //Number b{3};
        //bool c = a > b;
        //fprintf(stderr, "----%d\n", (a + b).getval());
        //printf("hello friend");
        /*
        for (int i = 0; i < 10; i ++)
        {
            NUMBER_ (a , i);
            //fprintf(stderr, "---------%s\n---------%s\n---------%d\n---------%s\n\n", a.info.name, a.info.file, a.info.line, a.info.constructor);
        }
        */
        //NUMBER_(d, NUMBER_(, "10"));
        //Number a( Number ("10"));
        //Number c (10);
        //Number d (Number a (10));
        //Number c =  (a == 4 + 3*a);
        //Number d = a + 3;


        //NUMBER_ (a, "10");
        //NUMBER_(c, a == 5 + a*4);
        //Number res (Number ("10"));
        NUMBER_(res, incr(sum(Number("10"), Number(3))));
        /*
        Number mass[10] = {3, 2, 1, 3, 4, 5, 7, 8, 9, 16};

        qsort(mass, 10, sizeof(Number), comp);
        for (int i = 0; i < 10; i ++)
            fprintf(stderr, "val:%d\n", mass[i].getval());
            */
        //Number c = a + 3;
        // SetConsoleColour(WHITE, CYAN);
        // COL oldColour = GetConsoleColour();
        // fprintf(stderr, "word: %u\n", sizeof(WORD));
        // SetConsoleColour(GREEN);
        // fprintf(stderr, "unsigned: %u\n", sizeof(unsigned));
        // fprintf(stderr, "DWORD: %u\n", sizeof(DWORD));
        // SetConsoleColour(oldColour & 0xFF, COL((oldColour & 0xFF00) >> 4));
        Number::printStat();
        SetConsoleColour(LIGHTCYAN);
        fprintf(stderr, "======== total objects: %d\n", Number::max_size);
        fprintf(stderr, "------------end of scope------------\n");
        SetConsoleColour(WHITE, BLACK);
    }
    getchar();
    return 0;
}


// счетчики сравнения и копирования + сортировки (своя и чужая)
// погрешность вычисления дабл числа
//  
// Homework 
// class array with [] with internal storage
// copy, call func with array, etc
//
// class vector with external storage
// do not delete copy constructor and operator = 
