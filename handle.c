#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "setup.h"
#include "handle.h"
#include "convert.h"
#include <time.h>

int DeckMinusHand(const int hands, char* hlist[], int deck[13][4], int tablearray[23][2]){
    char* check;
    int value;
    int suit;
    for(int i = 0; i < hands; i++){
        check = hlist[i];
        value = cvalue(*check++);
        suit = csuit(*check++);
        if(deck[value][suit] == 0){
            fprintf(stderr,"Error: Hand card duplicates");
            return EXIT_FAILURE;
        }
        deck[value][suit] = 0;
        tablearray[5+2*i][0] = value;
        tablearray[5+2*i][1] = suit;
        cardleft--;
        value = cvalue(*check++);
        suit = csuit(*check);
        if(deck[value][suit] == 0){
            fprintf(stderr,"Error: Hand card duplicates");
            return EXIT_FAILURE;
        }
        deck[value][suit] = 0;
        tablearray[6+2*i][0] = value;
        tablearray[6+2*i][1] = suit;
        cardleft--;
    }
    /*printf("Check Input Hand Deck\n");
        for(int val = 0; val < 13;val++){
            for(int sui = 0; sui < 4; sui++)
                printf("%d",deck[val][sui]);
            printf("\n");
        }
    */
   return EXIT_SUCCESS;
}

int DeckMinusComm(const int table, char* tlist[], int deck[13][4], int tablearray[23][2], int cdeck[14][5]){
    char* check;
    int value;
    int suit;
    printf("Tablecard    Value       Suit\n");
    for(int i = 0; i < tablecards; i++){
        check = tlist[i];
        printf("%s, ", check);
        value = cvalue(*check++);
        suit = csuit(*check);
        printf("         %d,         %d\n", value,suit);
        if(deck[value][suit] == 0){
            fprintf(stderr,"Error: Community cards duplicates");
            return EXIT_FAILURE;
        }
        deck[value][suit] = 0;
        tablearray[i][0] = value;
        tablearray[i][1] = suit;
        cdeck[value][suit] = 1;
        cdeck[value][4]++;
        cdeck[13][suit]++;
        cardleft--;
    }
    return EXIT_SUCCESS;
}

void rdealer(const int tablecards, int rundeck[13][4], int tablearray[23][2], int communitydeck[14][5]){
    int value;
    int suit;
    for(int i = tablecards; i < 5; i++){            
        do{
            int rval = rand() % 52;
            value = rval % 13;
            suit = rval/13;
        } while (rundeck[value][suit] == 0);
                tablearray[i][0] = value;
                tablearray[i][1] = suit;
                //printf("             %d,         %d\n",value + 2, suit);
                rundeck[value][suit] = 0;
                communitydeck[value][suit] = 1;
                communitydeck[value][4]++;
                communitydeck[13][suit]++;
        }
}

void handload(int i,int communitydeck[14][5], int tablearray[23][2]){
            if(i != 0){
                communitydeck[tablearray[3 + 2 * i][0]][tablearray[3 + 2 * i][1]] = 0;
                communitydeck[tablearray[4 + 2 * i][0]][tablearray[4 + 2 * i][1]] = 0;
                communitydeck[tablearray[3 + 2 * i][0]][4]--;
                communitydeck[13][tablearray[3 + 2 * i][1]]--;
                communitydeck[tablearray[4 + 2 * i][0]][4]--;
                communitydeck[13][tablearray[4 + 2 * i][1]]--;
                communitydeck[tablearray[5 + 2 * i][0]][tablearray[5 + 2 * i][1]] = 1;
                communitydeck[tablearray[6 + 2 * i][0]][tablearray[6 + 2 * i][1]] = 1;
                communitydeck[tablearray[5 + 2 * i][0]][4]++;
                communitydeck[13][tablearray[5 + 2 * i][1]]++;
                communitydeck[tablearray[6 + 2 * i][0]][4]++;
                communitydeck[13][tablearray[6 + 2 * i][1]]++;
            }else{
            communitydeck[tablearray[5][0]][tablearray[5][1]] = 1;
            communitydeck[tablearray[6][0]][tablearray[6][1]] = 1;
            communitydeck[tablearray[5][0]][4]++;
            communitydeck[13][tablearray[5][1]]++;
            communitydeck[tablearray[6][0]][4]++;
            communitydeck[13][tablearray[6][1]]++;
            }
}


void handlevel(int handnum,int communitydeck[14][5], int wtable[9][7]){
    enum strength{Hi, Pair, TwoPair, Three, Straight, Flush, FullHouse, Four, StraightFlush};
    int maxrepeatvalue = 0;
    int maxrepeatnum = 0;
    int secondhighv = 0;
    int secondhighn = 0;
    int maxrepeatsuit = 0;
    int highv[7] = {0};
    int max = 0;
    int ftemp = 0;
    int temp = 0;
    int cnt = 0; //count for high cards;
    int suit;
    int strv = 0;
    int strcnt = 0;
    int fstrcnt = 0;
    int strprev;
    int flushcnt = 0;
    int flush[5];
    int stra = 0 ;
    for(int s = 0; s < 4; s++){
        if(communitydeck[13][s] > maxrepeatsuit){
            maxrepeatsuit = communitydeck[13][s];
            suit = s;
        } 
    }
    for(int s = 0; s < 7; s++){
        wtable[handnum][s] = 0;
    }
    /*
    printf("Check Deck after dealt \n");
        for(int i = 0; i < 5;i++){
            for(int j = 0; j < 14; j++)
                printf("%d",communitydeck[j][i]);
                printf("\n");
        }*/
    for(int v = 12; v >= 0; v--){
        if(maxrepeatsuit >= 5){
            if(communitydeck[v][suit] == 1){
                flush[flushcnt++] = v;
                fstrcnt++;
                if((fstrcnt == 4 && v == 0) && communitydeck[12][suit] == 1){
                    fstrcnt++;
                    v = -1;
                }if(fstrcnt == 5){
                    wtable[handnum][0] = StraightFlush;
                    wtable[handnum][1] = 1;
                    wtable[handnum][2] = v + 2;
                    return;
                }
            }else{
                fstrcnt = 0;
            }
        }
        if(communitydeck[v][4] > maxrepeatnum){
            secondhighn = maxrepeatnum;
            secondhighv = maxrepeatvalue;
            maxrepeatnum = communitydeck[v][4];
            maxrepeatvalue = v;
        }else if(communitydeck[v][4] > secondhighn){
            secondhighn = communitydeck[v][4];
            secondhighv = v;
        }
        if(communitydeck[v][4] >= 1){
            strcnt = ++temp;
            if(strcnt == 5){
                strv = v;
                stra = 1;
            }
            highv[cnt] = v;
            cnt++;
        }else{
            temp = 0;
        }
    }
    //printf("Hand: %d, Max#: %d, Secondhigh#: %d\n",handnum,maxrepeatnum,secondhighn);
    /*for(int i = 0; i < 7; i++){
        printf("%d, ", highv[i]);
    }*/
    //printf("\n");
    if(maxrepeatsuit >= 5){
        wtable[handnum][0] = Flush;
        wtable[handnum][1] = 5;
        for(int i = 0; i < 5; i ++){
            wtable[handnum][i + 2] = flush[i] + 2;
        }
        return;
    }
    if(stra == 1){
            wtable[handnum][0] = Straight;
            wtable[handnum][1] = 1;
            wtable[handnum][2] = strv + 2;
            return;
    }
    switch (maxrepeatnum)
    {
    case 4:
        wtable[handnum][0] = Four;
        wtable[handnum][1] = 2;
        wtable[handnum][2] = maxrepeatvalue + 2;
        wtable[handnum][3] = highv[0] + 2;
        if(wtable[handnum][2] == wtable[handnum][3])
            wtable[handnum][3] = highv[1] + 2;
        break;
    case 3:
        if(secondhighn >= 2){
            wtable[handnum][0] = FullHouse;
            wtable[handnum][1] = 2;
            wtable[handnum][2] = maxrepeatvalue + 2;
            wtable[handnum][3] = secondhighv + 2;
        }else{
            wtable[handnum][0] = Three;
            wtable[handnum][1] = 3;
            wtable[handnum][2] = maxrepeatvalue + 2;
            if(wtable[handnum][2] == highv[0] + 2){
                wtable[handnum][3] = highv[1] + 2;
                wtable[handnum][4] = highv[2] + 2;
            }else if(wtable[handnum][2] == highv[1] + 2){
                wtable[handnum][3] = highv[0] + 2;
                wtable[handnum][4] = highv[2] + 2;
            }else{
                wtable[handnum][3] = highv[0] + 2;
                wtable[handnum][4] = highv[1] + 2;
            }
        }
        break;
    case 2:
        if(secondhighn == 2){
            wtable[handnum][0] = TwoPair;
            wtable[handnum][1] = 3;
            wtable[handnum][2] = maxrepeatvalue + 2;
            wtable[handnum][3] = secondhighv + 2;
            wtable[handnum][4] = highv[0] + 2;
            int i = 1;
            while(wtable[handnum][4] == secondhighv + 2 || wtable[handnum][4] == maxrepeatvalue + 2){
                wtable[handnum][4] = highv[i++] + 2;
            }
        }else{
            wtable[handnum][0] = Pair;
            wtable[handnum][1] = 4;
            wtable[handnum][2] = maxrepeatvalue + 2;
            int i = 3;
            int gap = 0;
            while(i != 6){
                if(highv[i-3] != maxrepeatvalue){
                    wtable[handnum][i] = highv[i - 3 + gap] + 2;
                }else{
                    wtable[handnum][i] = highv[i - 2] + 2;
                    gap = 1;
                }
                i++;
            }
        }
        break;
    case 1:
        wtable[handnum][0] = Hi;
        wtable[handnum][1] = 5;
        for(int i = 0; i < 5; i++){
            wtable[handnum][i + 2] = highv[i] + 2;
        }
        break;
    }
    return;
}

void recordrun(int wtable[9][7], int wtime[9]){
    int biggest[7] = {0};
    biggest[1] = 5;
    for(int i = 0; i < hands; i++){
        if(wtable[i][0] > biggest[0]){
            biggest[0] = wtable[i][0];
            biggest[1] = wtable[i][1];
        }
    }
    for(int i = 0; i < hands; i++){
        if(wtable[i][0] == biggest[0]){
            for(int j = 2; j < 2 + wtable[i][1]; j++){
                if(wtable[i][j] < biggest[j])
                    break;
                if(wtable[i][j] > biggest[j]){
                    while(j != 7){
                        biggest[j] = wtable[i][j];
                        j++;
                    }
                    break;
                }
            }
        }
    }
    int equal;
    /*
    printf("Win Hand:");
    for(int i = 0; i < 7; i++){
        printf("%d, ", biggest[i]);
    }
    printf("\n");
    */
    for(int i = 0; i < hands; i++){
        equal = 1;
        for(int j = 0; j < 7; j++){
            if(wtable[i][j] != biggest[j]){
                equal = 0;
                break;
            }
        }
        if(equal == 1){
            wtime[i]++;
        }
    }
}
