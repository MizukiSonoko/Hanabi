#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include <curses.h>

char* version = "1.2.0";

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

int get_color(){
    return (rand() % 7) + 1;
}

void fireworks(int time,int pos,int line_length,int color){
    int i = 0, j = 0;
    attrset(COLOR_PAIR(color) | A_BOLD);
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
    attrset(COLOR_PAIR(0));
}

/*
 * Generate a two-row mountain range into mountain_set.
 * mountain_set[0..width-1] is the upper row, mountain_set[width..2*width-1] the lower row.
 * The ridge is tracked as a level (0: ground, 1: lower row, 2: upper row) so that
 * every '/', '^' and '\' connects to its neighbour.
 */
void generate_mountain(int width,int* mountain_set){
    char texture[] = {' ', ' ', ' ', ' ', ' ', ' ', '.', ','};
    int *upper = mountain_set;
    int *lower = mountain_set + width;
    int level = 0;
    int x, c;

    for(x = 0;x < width;x++){
        upper[x] = 0;
        lower[x] = 0;
        c = rand() % 10;
        switch(level){
            case 0:
                if(c < 7){
                    lower[x] = '/';
                    level = 1;
                }
                break;
            case 1:
                if(c < 6){
                    upper[x] = '/';
                    lower[x] = ' ';
                    level = 2;
                }else if(c < 8){
                    lower[x] = '^';
                }else{
                    lower[x] = '\\';
                    level = 0;
                }
                break;
            case 2:
                lower[x] = texture[rand() % sizeof(texture)];
                if(c < 5){
                    upper[x] = '^';
                }else{
                    upper[x] = '\\';
                    level = 1;
                }
                break;
        }
    }
}

void mountain(int line,int width,int* mountain_set){
    int x;
    for(x = 0;x < width;x++){
        if(mountain_set[x]){
            mvaddch(line - 1, x, mountain_set[x]);
        }
        if(mountain_set[width + x]){
            mvaddch(line, x, mountain_set[width + x]);
        }
    }
}


int BOMB = 0;
bool INFINITE = false;
int TIME = 0;
int MOUNTAIN = 0;
int FREQUENCY = 0;

void opt(int argc,char *argv[]){
    int result;
    while((result=getopt(argc,argv,"f:t:b:imv")) != -1){
        switch(result){
            case 'm':        
                MOUNTAIN = 1;
                break;
            case 'b':        
                BOMB = atoi(optarg);
                break;
            case 'i':        
                INFINITE = true;
                break;
            case 'f':        
                FREQUENCY = atoi(optarg);
                break;
            case 't':        
                TIME = atoi(optarg);
                break;                
            case 'v':
                printf("  *|*                                           \n");
                printf(" *\\*/*   _   _                   _     _        \n");
                printf(" -*O*-  | | | | __ _ _ __   __ _| |__ (_)       \n");
                printf(" */*\\*  | |_| |/ _` | '_ \\ / _` | '_ \\| |       \n");
                printf("  *|*   |  _  | (_| | | | | (_| | |_) | |       \n");
                printf("   |    |_| |_|\\__,_|_| |_|\\__,_|_.__/|_|  %s\n", version);
                printf("   |                                            \n");
                printf("------------------------------------------------\n");
                printf("################################################\n");
                printf("\n           Hanabi version:%s\n\n", version);
                exit(0);              
            default: /* '?' */
                fprintf(stderr, "Usage: %s [-b bomb] [-t time] [-f fps] [-i] [-m] [-v]\n",
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
    start_color();

	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

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
        TIME = INT_MAX;
    }
    
    int *firework_set;
    int *firework_color_set;
    int *mountain_set;
    firework_set = malloc(sizeof(int) * number * 2);
    firework_color_set = malloc(sizeof(int) * number * 2);
    mountain_set = malloc(sizeof(int) * width * 2);

    if(MOUNTAIN){
        generate_mountain(width, mountain_set);
    }

    for(j=0;j < number * 2;j += 2){
        firework_set[j] = rand() % (heigth / 2) + (heigth / 3);
        firework_set[j+1] = 0;
        firework_color_set[j] = get_color();
        firework_color_set[j+1] = 0;
        bomb++;
    }

    for(i=0;i < TIME || INFINITE;i++){
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
                fireworks(firework_set[j+1]-firework_set[j]-1, j*8 ,firework_set[j], firework_color_set[j]);
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
    free(firework_color_set);
    free(mountain_set);
    return 0;
}
