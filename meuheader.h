#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef MEUMACRO
    #define MEUMACRO __declspec(dllexport)

#else 
    #define MEUMACRO __declspec(dllimport)
#endif