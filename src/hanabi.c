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


#define NIAGARA_SUMMIT_HALF 3
#define NIAGARA_MAX_LEVEL 6
#define NIAGARA_IGNITE_SPEED 2
#define NIAGARA_BURN_TIME 80
#define NIAGARA_REST_TIME 30

/* Half width of the Fuji-shaped wire at the given level (0: summit).
 * It grows quadratically so the slopes are steep near the top and gentle at the foot. */
int fuji_half_width(int level){
    return NIAGARA_SUMMIT_HALF + level * (level + 1) / 2;
}

int fuji_levels(int width,int wire_y){
    int levels = heigth / 8;
    if(levels > NIAGARA_MAX_LEVEL){
        levels = NIAGARA_MAX_LEVEL;
    }
    if(levels > wire_y - 1){
        levels = wire_y - 1;
    }
    while(levels > 0 && fuji_half_width(levels) * 2 + 1 > width){
        levels--;
    }
    return levels;
}

/* Returns the row of the wire at column x and stores the character to draw in wire_char. */
int niagara_wire(int x,int center,int wire_y,int levels,char* wire_char){
    int d = abs(x - center);
    int k;
    if(levels > 0 && d <= NIAGARA_SUMMIT_HALF){
        *wire_char = '_';
        return wire_y - levels;
    }
    for(k = 1;k <= levels && fuji_half_width(k) < d;k++);
    if(k > levels){
        *wire_char = '=';
        return wire_y;
    }
    if(d == fuji_half_width(k - 1) + 1){
        *wire_char = x < center ? '/' : '\\';
    }else{
        *wire_char = k == levels ? '=' : '_';
    }
    return wire_y - levels + k;
}

/* Sparks are a hash of (x, y - t) so the pattern flows down one row per frame.
 * They thin out as they fall away from the wire. */
char niagara_spark(int x,int y,int t,int dist,int span){
    unsigned int h = (unsigned int)x * 73856093u ^ (unsigned int)(y - t) * 19349663u;
    h ^= h >> 13;
    h *= 0x5bd1e995u;
    h ^= h >> 15;
    if((int)(h % 100) < dist * 60 / span){
        return ' ';
    }
    if(dist <= 2){
        return "||:'"[(h / 100) % 4];
    }
    return ":.'.,|"[(h / 100) % 6];
}

/* White above the flat wire (snow on Fuji), then yellow fading to red near the ground. */
int niagara_color(int y,int wire_y,int ground){
    if(y <= wire_y){
        return COLOR_PAIR(7) | A_BOLD;
    }
    if(y - wire_y < (ground - wire_y) / 2){
        return COLOR_PAIR(3) | A_BOLD;
    }
    return COLOR_PAIR(1) | A_BOLD;
}

int niagara_wire_y(){
    return heigth * 3 / 4;
}

int niagara_summit(int width){
    int wire_y = niagara_wire_y();
    return wire_y - fuji_levels(width, wire_y);
}

/* Draws frame i of a Niagara fireworks whose wire rises like Mt. Fuji in the middle.
 * The fire starts at the summit, spreads to both ends and burns out in the same order. */
void niagara(int i,int width){
    int wire_y = niagara_wire_y();
    int ground = heigth - 3;
    int levels = fuji_levels(width, wire_y);
    int center = width / 2;
    int cycle = center / NIAGARA_IGNITE_SPEED + NIAGARA_BURN_TIME + ground + NIAGARA_REST_TIME;
    int t = i % cycle;
    int x, y, top, lit, from;
    char wire_char, spark;

    for(x = 0;x < width;x++){
        top = niagara_wire(x, center, wire_y, levels, &wire_char);
        lit = t - abs(x - center) / NIAGARA_IGNITE_SPEED;

        attrset(lit >= 0 && lit < NIAGARA_BURN_TIME ? COLOR_PAIR(3) | A_BOLD : COLOR_PAIR(7));
        mvaddch(top, x, wire_char);
        if(lit < 0){
            continue;
        }

        /* The curtain front falls one row per frame; after burning out its top falls the same way. */
        from = lit > NIAGARA_BURN_TIME ? lit - NIAGARA_BURN_TIME : 0;
        for(y = top + 1 + from;y <= ground && y <= top + lit;y++){
            spark = niagara_spark(x, y, t, y - top, ground - wire_y + levels);
            if(spark == ' '){
                continue;
            }
            attrset(niagara_color(y, wire_y, ground));
            mvaddch(y, x, spark);
        }
    }
    attrset(COLOR_PAIR(0));
}


int BOMB = 0;
bool INFINITE = false;
int TIME = 0;
int MOUNTAIN = 0;
int NIAGARA = 0;
int FREQUENCY = 0;

void opt(int argc,char *argv[]){
    int result;
    while((result=getopt(argc,argv,"f:t:b:imnv")) != -1){
        switch(result){
            case 'm':        
                MOUNTAIN = 1;
                break;
            case 'n':
                NIAGARA = 1;
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
                fprintf(stderr, "Usage: %s [-b bomb] [-t time] [-f fps] [-i] [-m] [-n] [-v]\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }
}

/* A flower spans 4 rows above and 3 rows below row heigth - length.
 * With Niagara it is kept above the Fuji summit so the two never overlap. */
int launch_length(int base){
    int low, high;
    if(NIAGARA){
        low = heigth - niagara_summit(width) + 4;
        high = heigth - 4;
        if(low <= high){
            return low + rand() % (high - low + 1);
        }
    }
    return rand() % (heigth / 2) + base;
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
        firework_set[j] = launch_length(heigth / 3);
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
                    firework_set[j] = launch_length(heigth / 4);
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

        if(NIAGARA){
            niagara(i, width);
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
