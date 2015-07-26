#include <stdio.h>
#include <curses.h>
#include <unistd.h>


#define FLOWER4_1 "    .    "
#define FLOWER4_2 "  .....  "
#define FLOWER4_3 " ..   .. "
#define FLOWER4_4 ".*.   .+."
#define FLOWER4_5 "..     .."
#define FLOWER4_6 ".+.   .+."
#define FLOWER4_7 "  .. ..  "
#define FLOWER4_8 "    .    "

#define FLOWER3_1 "    .    "
#define FLOWER3_2 "  ..*..  "
#define FLOWER3_3 " .+* *+. "
#define FLOWER3_4 ".*.. ..+."
#define FLOWER3_5 "...   .;."
#define FLOWER3_6 ".+*. .*+."
#define FLOWER3_7 "  .....  "
#define FLOWER3_8 "    .    "

#define FLOWER2_1 "    .    "
#define FLOWER2_2 "  ..|..  "
#define FLOWER2_3 " ..*|*.. "
#define FLOWER2_4 ".>*\\*/*<."
#define FLOWER2_5 ".--*O*--."
#define FLOWER2_6 ".>*/*\\*<."
#define FLOWER2_7 "  .*|*.  "
#define FLOWER2_8 "   ^|^   "

#define FLOWER1_1 "   *|*   "
#define FLOWER1_2 "  *\\*/*  "
#define FLOWER1_3 "  -*O*-  "
#define FLOWER1_4 "  */*\\*  "
#define FLOWER1_5 "   *|*   "

#define FLOWER0_1 "   \\*/   "
#define FLOWER0_2 "   *O*   "
#define FLOWER0_3 "   /*\\   "


#define BALL    "    O    "
#define LINE    "    |    "
#define SKY     "         "
#define HORIZON "---------"
#define EARTH   "#########"


void println(int start,int line,char* str){
   int i =start;
   for(;*str!='\0';++i){
        mvaddch(line, i, *str);
        str++;
    }
}

int printSp(int start,int num,char* str){
    int i;
    for(i = start;i < start + num; i++){
        println(0, i,str);
    }
    return num;
}

void fireworks(int time){
    int i = 0, j = 0;
    switch(time){
        case 25:case 26:
            i += printSp(0, 2, SKY);
            i += printSp(i, 1, FLOWER4_1);
            i += printSp(i, 1, FLOWER4_2);
            i += printSp(i, 1, FLOWER4_3);
            i += printSp(i, 1, FLOWER4_4);
            i += printSp(i, 1, FLOWER4_5);
            i += printSp(i, 1, FLOWER4_6);
            i += printSp(i, 1, FLOWER4_7);
            i += printSp(i, 1, FLOWER4_8);
            i += printSp(i, 4, SKY);
            i += printSp(i, 1, HORIZON);
            i += printSp(i, 1, EARTH);    break;
        case 22:case 23:case 24:
            i += printSp(0, 2, SKY);
            i += printSp(i, 1, FLOWER3_1);
            i += printSp(i, 1, FLOWER3_2);
            i += printSp(i, 1, FLOWER3_3);
            i += printSp(i, 1, FLOWER3_4);
            i += printSp(i, 1, FLOWER3_5);
            i += printSp(i, 1, FLOWER3_6);
            i += printSp(i, 1, FLOWER3_7);
            i += printSp(i, 1, FLOWER3_8);
            i += printSp(i, 1, LINE);
            i += printSp(i, 3, SKY);
            i += printSp(i, 1, HORIZON);
            i += printSp(i, 1, EARTH);    break;
        case 17:case 18:case 19:case 20:case 21:
            i += printSp(0, 2, SKY);
            i += printSp(i, 1, FLOWER2_1);
            i += printSp(i, 1, FLOWER2_2);
            i += printSp(i, 1, FLOWER2_3);
            i += printSp(i, 1, FLOWER2_4);
            i += printSp(i, 1, FLOWER2_5);
            i += printSp(i, 1, FLOWER2_6);
            i += printSp(i, 1, FLOWER2_7);
            i += printSp(i, 1, FLOWER2_8);
            i += printSp(i, 4, LINE);
            i += printSp(i, 1, HORIZON);
            i += printSp(i, 1, EARTH);    break;
        case 14:case 15:case 16:
            i += printSp(0, 4, SKY);
            i += printSp(i, 1, FLOWER1_1);
            i += printSp(i, 1, FLOWER1_2);
            i += printSp(i, 1, FLOWER1_3);
            i += printSp(i, 1, FLOWER1_4);
            i += printSp(i, 1, FLOWER1_5);
            i += printSp(i, 5, LINE);
            i += printSp(i, 1, HORIZON);
            i += printSp(i, 1, EARTH);    break;
        case 12:case 13:
            i += printSp(0, 5, SKY);
            i += printSp(i, 1, FLOWER0_1);
            i += printSp(i, 1, FLOWER0_2);
            i += printSp(i, 1, FLOWER0_3);
            i += printSp(i, 6, LINE);
            i += printSp(i, 1, HORIZON);
            i += printSp(i, 1, EARTH);    break;
        case 10:case 11:
            i += printSp(0, 6, SKY);
            i += printSp(i, 1, BALL);
            i += printSp(i, 7, LINE);
            i += printSp(i, 1, HORIZON);
            i += printSp(i, 1, EARTH);    break;
        case 9:
            i += printSp(0, 7, SKY);
            i += printSp(i, 7, LINE);
            i += printSp(i, 1, HORIZON);
            i += printSp(i, 1, EARTH);    break;
        case 8:
            i += printSp(0, 8, SKY);
            i += printSp(i, 6, LINE);
            i += printSp(i, 1, HORIZON);
            i += printSp(i, 1, EARTH);    break;
        case 7:
            i += printSp(0, 9, SKY);
            i += printSp(i, 5, LINE);
            i += printSp(i, 1, HORIZON);
            i += printSp(i, 1, EARTH);    break;
        case 6:
            i += printSp(0,10, SKY);
            i += printSp(i, 4, LINE);
            i += printSp(i, 1, HORIZON);
            i += printSp(i, 1, EARTH);    break;
        case 5:
            i += printSp(0,11, SKY);
            i += printSp(i, 3, LINE);
            i += printSp(i, 1, HORIZON);
            i += printSp(i, 1, EARTH);    break;
        case 4:
            i += printSp(0,12, SKY);
            i += printSp(i, 2, LINE);
            i += printSp(i, 1, HORIZON);
            i += printSp(i, 1, EARTH);    break;
        case 3:
            i += printSp(0,13, SKY);
            i += printSp(i, 1, LINE);
            i += printSp(i, 1,HORIZON);
            i += printSp(i, 1,EARTH);  break;
        default:
            i += printSp(0,14,SKY);
            i += printSp(i, 1,HORIZON);
            i += printSp(i, 1,EARTH);
    }
}


int main(){
    initscr();
    noecho();
    int i, M=30;
    for(i=0;i<M;i++){
        erase();
        fireworks(i);
        refresh();
        usleep(300000);
    }
    endwin();
    return 0;
}

