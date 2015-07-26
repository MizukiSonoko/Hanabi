#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

char* FLOWER4[] ={
 "         ",
 "  .....  ",
 " ..   .. ",
 ".*.   .+.",
 "..     ..",
 ".+.   .+.",
 "  .. ..  ",
 "    .    "
};

char* FLOWER3[] ={
 "         ",
 "  ..*..  ",
 " .+* *+. ",
 ".*.. ..+.",
 "...   .;.",
 ".+*. .*+.",
 " ..;.;.. ",
 "    .    "
};

char* FLOWER2[] ={
 "         ",
 "  ..|..  ",
 " ..*|*.. ",
 ".>*\\*/*<.",
 ".--*O*--.",
 ".>*/*\\*<.",
 " /.*|*.\\ ",
 "   ^|^   "
};

char* FLOWER1[] ={
 "   *|*   ",
 "  *\\*/*  ",
 "  -*O*-  ",
 "  */*\\*  ",
 "   *|*   "
};

char* FLOWER0[] ={
 "   \\*/   ",
 "   *O*   ",
 "   /*\\   "
};

char* BALL =  "    O    ";
char* LINE =  "    |    ";
char* SKY  =  "         ";

int width, heigth;

void printCline(int start,int line,int num, char c){
    int i = start;
    for(;i < num; i++){
        mvaddch(line, i, c);
    }
}
void println(int start,int line,char* str){
   int i =start;
   for(;*str!='\0';++i){
        mvaddch(line, i, *str);
        str++;
    }
}
int printSp(int start,int line,int num,char* str){
    int i;
    for(i = line;i < line+num; i++){
        println(start, i,str);
    }
    return num;
}

void set_off(int pos,int length,int heigth){
    printSp(pos, heigth - length, length, LINE);
}

void fireworks(int time,int pos,int line_length){
    int i = 0, j = 0;
    switch(time){
        case 16:case 17:
            for(j=0;j < 8;j++){
                printSp(pos, heigth - line_length - 4 + j, 1, FLOWER4[j]);
            }
            break;
        case 13:case 14:case 15:
            for(j=0;j < 8;j++){
                printSp(pos, heigth - line_length - 4 + j, 1, FLOWER3[j]);
            }
            printSp(pos, heigth - line_length + 4, 2, LINE); break;
        case 8:case 9:case 10:case 11:case 12:
            for(j=0;j < 8;j++){
                printSp(pos, heigth - line_length - 4 + j, 1, FLOWER2[j]);
            }
            printSp(pos, heigth - line_length + 4, 3, LINE); break;
        case 5:case 6:case 7:
            for(j=0;j < 5;j++){
                printSp(pos, heigth - line_length - 2 + j, 1, FLOWER1[j]);
            }
            printSp(pos, heigth - line_length + 3, line_length/8, LINE); break;
        case 3:case 4:
            for(j=0;j < 3;j++){
                printSp(pos, heigth - line_length - 1 + j, 1, FLOWER0[j]);
            }
            printSp(pos, heigth - line_length + 2, line_length/5, LINE); break;
        case 1:case 2:
            printSp(pos, heigth - line_length, 1, BALL);
            printSp(pos, heigth - line_length + 1, line_length/2, LINE);
            break;
    }
}


int main(){
    initscr();
    noecho();
    curs_set(0);
    getmaxyx(stdscr, heigth, width);
    
    int number = width / 8;
    int i, j;
    
    int *firework_set;
    firework_set = malloc(sizeof(int) * number * 2);
    
    srand((unsigned)time(NULL));
    
    for(j=0;j < number;j+=2){
        firework_set[j] = rand() % (heigth / 2) + (heigth / 4);
        firework_set[j+1] = 0;
    }

    for(i=0;i < 300;i++){
        erase();

        for(j=0;j < number;j+=2){

            if(firework_set[j+1] == firework_set[j] + 17){
                if(rand() % 3){
                    firework_set[j] = rand() % (heigth / 2) + (heigth / 4);
                    firework_set[j+1] = 0;
                }
            }else{
                firework_set[j+1]++;
            }

            if(firework_set[j+1] < firework_set[j]){
                set_off( j*8 ,firework_set[j+1],heigth);
            }else{
                fireworks(firework_set[j+1]-firework_set[j]-1, j*8 ,firework_set[j]);
            }
        }

        printCline( 0, heigth - 2, width, '-');
        printCline( 0, heigth - 1, width, '#');
        
        refresh();
        usleep(300000);
    }
    endwin();
    free(firework_set);
    return 0;
}

