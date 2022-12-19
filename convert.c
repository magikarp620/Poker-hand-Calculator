#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "convert.h"

int csuit(const char a)
{
    switch(a)
    {
        case 'c': return 0;
        case 'd': return 1;
        case 'h': return 2;
        case 's': return 3;
        case 'C': return 0;
        case 'D': return 1;
        case 'H': return 2;
        case 'S': return 3;
        case '?': 
        default:
            fprintf(stderr, "ERROR: Suit Input incorrect");
            return EXIT_FAIL;
    }
}
int cvalue(const char b){
    switch(b)
    {
        case '2': return 0;
        case '3': return 1;
        case '4': return 2;
        case '5': return 3;
        case '6': return 4;
        case '7': return 5;
        case '8': return 6;
        case '9': return 7;
        case 'T': return 8;
        case 'J': return 9;
        case 'Q': return 10;
        case 'K': return 11;
        case 'A': return 12;
        case '?': 
        default:
            fprintf(stderr, "ERROR: Value Input incorrect");
            return EXIT_FAIL;
    }
}
