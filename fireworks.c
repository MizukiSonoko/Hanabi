#include <stdio.h>
#include <curses.h>
#include <unistd.h>

#define FLOWER1 " *|*　"
#define FLOWER2 "*\*/* "
#define FLOWER3 "─*O*- "
#define FLOWER4 "*/※\* "
#define FLOWER5 " *|*  "

#define BALL    "  O   "
#define LINE    "  |   "
#define SKY     "      "
#define HORIZON "------"
#define EARTH   "######"


void println(int line,char* str){
   int i =0;
   for(;*str!='\0';++i){
        mvaddch(line, i, *str);
        str++;
    }
}

int printSp(int start,int num,char* str){
    int i;
    for(i = start;i < start + num; i++){
        println(i,str);
    }
    return num;
}

void fireworks(int time){
    int i = 0, j = 0;
    switch(time){
        case 9:
            i += printSp(0, 7, SKY);
            i += printSp(i, 7, LINE);
            i += printSp(i, 1, HORIZON);
            println(i,EARTH);    break;
        case 8:
            i += printSp(0, 8, SKY);
            i += printSp(i, 6, LINE);
            i += printSp(i, 1, HORIZON);
            println(i,EARTH);    break;
        case 7:
            i += printSp(0, 9, SKY);
            i += printSp(i, 5, LINE);
            i += printSp(i, 1, HORIZON);
            println(i,EARTH);    break;
        case 6:
            i += printSp(0,10, SKY);
            i += printSp(i, 4, LINE);
            i += printSp(i, 1, HORIZON);
            println(i,EARTH);    break;
        case 5:
            i += printSp(0,11, SKY);
            i += printSp(i, 3, LINE);
            i += printSp(i, 1, HORIZON);
            println(i,EARTH);    break;
        case 4:
            i += printSp(0,12, SKY);
            i += printSp(i, 2, LINE);
            i += printSp(i, 1, HORIZON);
            println(i,EARTH);    break;
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
    int i, M=10;
    for(i=0;i<M;i++){
        erase();
        fireworks(i);
        refresh();
        usleep(500000);
    }
    endwin();
    return 0;
}

