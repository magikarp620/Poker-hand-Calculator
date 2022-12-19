#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "setup.h"

int setup(int argc, char *argv[], char*hlist[], char*tlist[])
{
    int opt;
    int hflag = 0;    //Make sure hand was input
    char *table;
    char *check;
    char temp[2];     //Array that temporay stores the hand

    while((opt = getopt(argc, argv, "h:t:")) != -1) {
        switch(opt){
            case 'h':
            //read hands
                hflag = 1;
                hands = atoi(optarg);
                if(hands <= 1 || hands > 9){
                    fprintf(stderr, "ERROR: Must have more than 1 and less than 10 hands");
                    return EXIT_FAIL;
                }
                for(int i = 0; i < hands; i++){
                    check = argv[optind + i];
                    if(strlen(check) != 4){
                        fprintf(stderr, "ERROR: Incorrect hand length");
                        return EXIT_FAIL;
                    }
                    hlist[i] = argv[optind + i]; 
                }
                optind+=hands;
                break;
            case 't':
                table = optarg;
                tablecards = strlen(table);
                if(tablecards % 2 != 0){
                        fprintf(stderr, "ERROR: Incorrect hand length");
                        return EXIT_FAIL;
                }
                tablecards = tablecards/2;
                //if(tablecards != 3 && tablecards != 4){
                //    fprintf(stderr, "ERROR: Community card can only be 0, 3 or 4");
                //    return EXIT_FAIL;
                //}
                for(int i = 0; i < tablecards; i++){
                    temp[0] = *table++;
                    temp[1] = *table++;
                    tlist[i] = strndup(temp,2);
                    //printf("%s\n", tlist[i]);
                }
                break;
            case '?':
            default:
                fprintf(stderr, "ERROR: Unrecognized input");
                return EXIT_FAIL;

        }
    }
    if(hflag == 0){
        fprintf(stderr, "ERROR: No hand inputted");
        return EXIT_FAIL;
    }
    return EXIT_SUCCESS;
}