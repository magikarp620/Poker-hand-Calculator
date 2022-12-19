#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <getopt.h>
#include <time.h>
#include "handle.h"
#include "setup.h"
#include "convert.h"
#include "main.h"


int main(int argc, char **argv){
    clock_t start, end;
    double execution_time;
    start = clock();
    char *table;
    char* check;
    char* handlist[9]; //Array that stores each player's hand 
    char* tablelist[5];//Array that stores table card
    int deck[13][4];   //Card deck that specify which cards are dealt
    int cdeck[14][5] = {0};
    int rundeck[13][4];
    int tablearray[23][2];
    int wintable[9][7] = {0};
    int communitydeck[14][5];
    int cardleft = 52;
    int wintime[9] = {0};
    int suit; int value; int temp; int high = 0;
    //Random seed every run
    srand(time(NULL));
    //Read command flag
    if(setup(argc, argv, handlist, tablelist) == EXIT_FAIL){
        fprintf(stderr, "ERROR");
        return EXIT_FAILURE;
    }
    //Create normal deck
    for(int i = 0; i < 13;i++){
        for(int j = 0; j < 4; j++)
            deck[i][j] = 1;
    }
    //Print hand list 
    printf("Hand      Player\n");
    for(int i = 0; i < hands; i++ ){
        printf("%s        %d\n", handlist[i], i+1);
    }
    //Take out hand card from deck
    if(DeckMinusHand(hands, handlist, deck, tablearray) != 0){
        fprintf(stderr,"Minus hand error");
        return EXIT_FAILURE;
    }
    //Take out community card from deck
    if(DeckMinusComm(tablecards, tablelist, deck, tablearray, cdeck) != 0){
        fprintf(stderr,"Minus Comm error");
        return EXIT_FAILURE;
    }
    //Check Deck with print 
    /*
    printf("Check Input Communnity Deck\n");
    for(int i = 0; i < 14;i++){
        for(int j = 0; j < 5; j++)
            printf("%d",cdeck[i][j]);
        printf("\n");
    }
    printf("Check Input Card Table Array\n");
    for(int i = 0; i < 9; i++){
        printf("%d",tablearray[i][0]);
        printf(", %d\n",tablearray[i][1]);
    }   
    */
   //Create copy deck to reiterate
    
    int totalrun = 1000000;
    srand(time(NULL));
    for(int loops = 0; loops < totalrun; loops++){
            for(int i = 0; i < 13; i++){
                for(int c = 0; c < 4; c++){
                    rundeck[i][c] = deck[i][c];
                }
            }
            for(int i = 0; i < 14; i++){
                for(int c = 0; c < 5; c++){
                    communitydeck[i][c] = cdeck[i][c];
                }
            }
        rdealer(tablecards, rundeck, tablearray, communitydeck);
        //for(int h = 0; h < 9; h++){
           // printf();
        //}
        for(int h = 0; h < hands; h++){
            handload(h, communitydeck, tablearray);
            handlevel(h, communitydeck, wintable);
            /*
            printf("Check Hand: %d Input Communnity Deck\n", h);
            for(int i = 0; i < 14;i++){
                for(int j = 0; j < 5; j++)
                    printf("%d",communitydeck[i][j]);
                printf("\n");
            }
            printf("Hand #%d: ", h + 1);
            for(int i = 0; i < 7; i++){
                printf("%d, ", wintable[h][i]);
            }
            printf("\n");
            */
        }
        recordrun(wintable, wintime);
        /*
        printf("Check table array\n");
        for(int i = 0; i < 9; i++){
            printf("%d",tablearray[i][0] + 2);
            printf(", %d\n",tablearray[i][1]);
        } 
        */  
    }
    for(int i = 0; i < hands; i++){
        float wpercent = wintime[i]/(float)totalrun;
        printf("Hand %d: Won %d time, win rate: %.2f %%\n", i + 1, wintime[i], wpercent* 100);
    }
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    printf("Runtime: %f\n", execution_time );
}

