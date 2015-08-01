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

void mountain(int line,int width,int* mountain_set){
    int i = 0, j = 0;
    int c, h = 0;

    /*
    if(mountain_set[0] != 0){
        for(;j < width * 2;j += 2){
            mvaddch( line, mountain_set[j+1], (char)mountain_set[j]);
        }
        return;
    }
    */

    for(;j < width * 2; j += 2){
        if(h == 0){
            mountain_set[j] = '/';
            mountain_set[j+1] = h;
            h++;
        }else if(h == 3){
            c = rand() % 3;
            if(c == 0){
                h--;
                mountain_set[j] = '^';
                mountain_set[j+1] = h;
            }else{
                mountain_set[j] = '\\';
                mountain_set[j+1] = h;
                h--;
            }
        }else{
            c = rand() % 3;
            if(c == 0){
                h--;
                mountain_set[j] = '^';
                mountain_set[j+1] = h;
            }else if(c == 1){
                h++;
                mountain_set[j] = '/';
                mountain_set[j+1] = h;
            }else{
                mountain_set[j] = '\\';
                mountain_set[j+1] = h;
                h--;
            }
        }
    }

    for(;j < width * 2;j += 2){
        mvaddch( line, mountain_set[j+1], (char)mountain_set[j]);
    }
}


int BOMB = 0;
int TIME = 0;
int MOUNTAIN = 0;
int FREQUENCY = 0;

void opt(int argc,char *argv[]){
    int result;
    while((result=getopt(argc,argv,"f:t:b:m")) != -1){
        switch(result){
            case 'm':        
                MOUNTAIN = 1;
                break;
            case 'b':        
                BOMB = atoi(optarg);
                break;
            case 'f':        
                FREQUENCY = atoi(optarg);
                break;
            case 't':        
                TIME = atoi(optarg);
                break;                
            default: /* '?' */
                fprintf(stderr, "Usage: %s [-b bomb] [-t time] [-f fps]\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }
}

int main(int argc,char *argv[]){
    opt( argc, argv);

    initscr();
    noecho();
    curs_set(0);
    getmaxyx(stdscr, heigth, width);
    
    int number = width / 8;
    int bomb = 0;
    int i, j;
    
    srand((unsigned)time(NULL));
    
    if(BOMB && number > BOMB){
        number = BOMB;
    }
    if(!TIME){
        TIME = 300;
    }
    if(!FREQUENCY){
        FREQUENCY = 10;
    }
    FREQUENCY *= 10000;

    if(BOMB){
        TIME = 1000000;
    }

    int *firework_set;
    int *mountain_set;
    firework_set = malloc(sizeof(int) * number * 2);
    mountain_set = malloc(sizeof(int) * width * 2);

    for(j=0;j < width * 2;j++){
        mountain_set[j] = 0;
    }

    for(j=0;j < number * 2;j += 2){
        firework_set[j] = rand() % (heigth / 2) + (heigth / 2);
        firework_set[j+1] = 0;
        bomb++;
    }

    for(i=0;i < TIME;i++){
        erase();

        for(j=0;j < number;j += 2){

            if(firework_set[j+1] == firework_set[j] + 17){            
                bomb++;
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

        if(MOUNTAIN){
            mountain(heigth - 3, width, mountain_set);
        }
        printCline( 0, heigth - 2, width, '-');
        printCline( 0, heigth - 1, width, '#');
        
        refresh();
        usleep(FREQUENCY);

        if(BOMB && bomb >= BOMB * 2){
            break;
        }
    }
    endwin();
    free(firework_set);
    free(mountain_set);
    return 0;
}
