#include <stdlib.h>
#include <curses.h>

#include "hanabi.h"
#include "special.h"

#define SHAPE_CHANCE 8          /* 1 in N launches blooms into a shape */
#define STARMINE_CHANCE 200     /* 1 in N frames starts a star mine */
#define BIG_CHANCE 250          /* 1 in N frames launches a big shell */
#define STARMINE_FRAMES 40
#define MAX_BURSTS 64
#define BURST_HOLD 4
#define BURST_FADE 6
#define BIG_MAX_RADIUS 10
#define BIG_MIN_RADIUS 4
#define RISE_SPEED 2
#define RAYS 24

/* Shaped fireworks (katamono) share the 9x8 footprint of the normal flowers. */
static const char* SHAPES[][8] = {
    {
     "         ",
     " **   ** ",
     "*  * *  *",
     "*   *   *",
     " *     * ",
     "  *   *  ",
     "   * *   ",
     "    *    "
    },
    {
     "  *****  ",
     " *     * ",
     "*  o o  *",
     "*       *",
     "* \\___/ *",
     " *     * ",
     "  *****  ",
     "         "
    },
    {
     "    *    ",
     "   ***   ",
     "*********",
     " ******* ",
     "  *****  ",
     " *** *** ",
     "**     **",
     "         "
    },
    {
     "/\\     /\\",
     "| \\___/ |",
     "| o   o |",
     "|   w   |",
     " \\     / ",
     "  `---'  ",
     "         ",
     "         "
    }
};
#define SHAPE_COUNT (int)(sizeof(SHAPES) / sizeof(SHAPES[0]))

/* Unit vectors (x1000) for 24 directions, so no libm is needed. */
static const int DIRECTIONS[RAYS][2] = {
    {1000, 0}, {966, 259}, {866, 500}, {707, 707}, {500, 866}, {259, 966},
    {0, 1000}, {-259, 966}, {-500, 866}, {-707, 707}, {-866, 500}, {-966, 259},
    {-1000, 0}, {-966, -259}, {-866, -500}, {-707, -707}, {-500, -866}, {-259, -966},
    {0, -1000}, {259, -966}, {500, -866}, {707, -707}, {866, -500}, {966, -259}
};

typedef struct {
    int cx, cy;
    int radius;
    int age;        /* negative while the rocket is still rising */
    int color;
    int pistil;     /* inner ring color for big shells, 0 for none */
} Burst;

static Burst bursts[MAX_BURSTS];
static int burst_count = 0;
static int starmine_left = 0;
static int floor_limit = 0;

int pick_shape(void){
    if(rand() % SHAPE_CHANCE){
        return SHAPE_NONE;
    }
    return rand() % SHAPE_COUNT;
}

void draw_shape(int shape,int time,int pos,int center){
    int row, col;
    char c;
    for(row = 0;row < 8;row++){
        for(col = 0;SHAPES[shape][row][col] != '\0';col++){
            c = SHAPES[shape][row][col];
            if(c == ' '){
                continue;
            }
            if(time >= 16){
                c = '.';
            }else if(c == '*' && (row + col + time) % 4 == 0){
                c = '+';
            }
            mvaddch(center - 4 + row, pos + col, c);
        }
    }
}

static void plot(int y,int x,char c){
    if(y >= 0 && y <= floor_limit && x >= 0 && x < width){
        mvaddch(y, x, c);
    }
}

static int random_between(int low,int high){
    if(high <= low){
        return low;
    }
    return low + rand() % (high - low + 1);
}

static void add_burst(int cx,int cy,int radius,int rise,int color,int pistil){
    Burst b;
    if(burst_count >= MAX_BURSTS){
        return;
    }
    b.cx = cx;
    b.cy = cy;
    b.radius = radius;
    b.age = -rise;
    b.color = color;
    b.pistil = pistil;
    bursts[burst_count++] = b;
}

static int other_color(int color){
    return color % 7 + 1;
}

void start_starmine(void){
    starmine_left = STARMINE_FRAMES;
}

/* A nishakudama: one huge shell rising from the ground with a two-tone core. */
void start_big(int floor_row){
    int radius = (floor_row - 3) / 2;
    int cy, cx, color;
    if(radius > BIG_MAX_RADIUS){
        radius = BIG_MAX_RADIUS;
    }
    if(radius > width / 4 - 1){
        radius = width / 4 - 1;
    }
    if(radius < BIG_MIN_RADIUS){
        return;
    }
    cy = (floor_row - 3) / 2;
    cx = random_between(radius * 2, width - radius * 2 - 1);
    color = get_color();
    add_burst(cx, cy, radius, (floor_row - cy) / RISE_SPEED, color, other_color(color));
}

static void spawn_starmine_burst(int floor_row){
    int radius = 2 + rand() % 2;
    int cy = random_between(radius, floor_row - radius - 3);
    int cx = random_between(radius * 2, width - radius * 2 - 1);
    add_burst(cx, cy, radius, 2, get_color(), 0);
}

static void draw_rocket(const Burst* b){
    int y = b->cy - b->age * RISE_SPEED;
    attrset(COLOR_PAIR(7) | A_BOLD);
    plot(y, b->cx, 'o');
    plot(y + 1, b->cx, '|');
    plot(y + 2, b->cx, '|');
}

static char ray_char(int dir,int head,int twinkle){
    int dx = DIRECTIONS[dir][0], dy = DIRECTIONS[dir][1];
    if(head){
        return twinkle ? '+' : '*';
    }
    if(abs(dy) < 380){
        return '-';
    }
    if(abs(dx) < 380){
        return '|';
    }
    return (dx > 0) == (dy > 0) ? '\\' : '/';
}

/* Draws one ray from the inner half of the radius out to its head.
 * It walks in half rows because a column is half as tall as a row,
 * otherwise horizontal rays would be dotted. While fading, sparks drop and thin out. */
static void draw_ray(const Burst* b,int dir,int r,int fade,int heads_only){
    int half, x, y;
    char c;
    for(half = heads_only ? r * 2 : r;half <= r * 2;half++){
        x = b->cx + DIRECTIONS[dir][0] * half / 1000;
        y = b->cy + DIRECTIONS[dir][1] * half / 2000;
        if(fade > 0){
            if((int)(hash2(dir * 31 + half, fade) % BURST_FADE) < fade){
                continue;
            }
            plot(y + fade * fade / 4, x, '.');
            continue;
        }
        c = ray_char(dir, half == r * 2, (dir + b->age) % 3 == 0);
        plot(y, x, c);
    }
}

static void draw_burst(const Burst* b){
    int r, fade, dir;
    int step = b->radius >= 6 ? 1 : 3;
    if(b->age < 0){
        draw_rocket(b);
        return;
    }
    r = b->age < b->radius ? b->age + 1 : b->radius;
    fade = b->age - b->radius - BURST_HOLD;
    attrset(COLOR_PAIR(b->color) | A_BOLD);
    for(dir = 0;dir < RAYS;dir += step){
        draw_ray(b, dir, r, fade, 0);
    }
    if(b->pistil && r >= 2){
        attrset(COLOR_PAIR(b->pistil) | A_BOLD);
        for(dir = 1;dir < RAYS;dir += 2){
            draw_ray(b, dir, r / 2, fade, 1);
        }
    }
}

static int burst_alive(const Burst* b){
    return b->age < b->radius + BURST_HOLD + BURST_FADE;
}

/* Star mines and big shells start at random, one event at a time.
 * Nothing is drawn below floor_row so they stay clear of Niagara. */
void special_fireworks(int floor_row){
    int i, alive = 0;
    floor_limit = floor_row;

    if(starmine_left == 0 && burst_count == 0){
        if(rand() % STARMINE_CHANCE == 0){
            start_starmine();
        }else if(rand() % BIG_CHANCE == 0){
            start_big(floor_row);
        }
    }
    if(starmine_left > 0){
        starmine_left--;
        spawn_starmine_burst(floor_row);
        if(rand() % 2){
            spawn_starmine_burst(floor_row);
        }
    }

    for(i = 0;i < burst_count;i++){
        draw_burst(&bursts[i]);
        bursts[i].age++;
        if(burst_alive(&bursts[i])){
            bursts[alive++] = bursts[i];
        }
    }
    burst_count = alive;
    attrset(COLOR_PAIR(0));
}
