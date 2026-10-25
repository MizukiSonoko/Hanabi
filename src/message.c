#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <curses.h>

#include "hanabi.h"
#include "message.h"

#define FONT_ROWS 5
#define GLYPH_WIDTH 6           /* 5 columns plus 1 column of spacing */
#define MESSAGE_RISE_SPEED 2
#define MESSAGE_REVEAL_SPEED 3  /* columns per frame */
#define MESSAGE_HOLD 30
#define MESSAGE_FADE 8
#define MESSAGE_REST 40

typedef struct {
    char c;
    const char* rows[FONT_ROWS];
} Glyph;

static const Glyph FONT[] = {
    {'A', {" ### ", "#   #", "#####", "#   #", "#   #"}},
    {'B', {"#### ", "#   #", "#### ", "#   #", "#### "}},
    {'C', {" ####", "#    ", "#    ", "#    ", " ####"}},
    {'D', {"#### ", "#   #", "#   #", "#   #", "#### "}},
    {'E', {"#####", "#    ", "#### ", "#    ", "#####"}},
    {'F', {"#####", "#    ", "#### ", "#    ", "#    "}},
    {'G', {" ####", "#    ", "#  ##", "#   #", " ### "}},
    {'H', {"#   #", "#   #", "#####", "#   #", "#   #"}},
    {'I', {"#####", "  #  ", "  #  ", "  #  ", "#####"}},
    {'J', {"#####", "   # ", "   # ", "#  # ", " ##  "}},
    {'K', {"#   #", "#  # ", "###  ", "#  # ", "#   #"}},
    {'L', {"#    ", "#    ", "#    ", "#    ", "#####"}},
    {'M', {"#   #", "## ##", "# # #", "#   #", "#   #"}},
    {'N', {"#   #", "##  #", "# # #", "#  ##", "#   #"}},
    {'O', {" ### ", "#   #", "#   #", "#   #", " ### "}},
    {'P', {"#### ", "#   #", "#### ", "#    ", "#    "}},
    {'Q', {" ### ", "#   #", "# # #", "#  # ", " ## #"}},
    {'R', {"#### ", "#   #", "#### ", "#  # ", "#   #"}},
    {'S', {" ####", "#    ", " ### ", "    #", "#### "}},
    {'T', {"#####", "  #  ", "  #  ", "  #  ", "  #  "}},
    {'U', {"#   #", "#   #", "#   #", "#   #", " ### "}},
    {'V', {"#   #", "#   #", "#   #", " # # ", "  #  "}},
    {'W', {"#   #", "#   #", "# # #", "## ##", "#   #"}},
    {'X', {"#   #", " # # ", "  #  ", " # # ", "#   #"}},
    {'Y', {"#   #", " # # ", "  #  ", "  #  ", "  #  "}},
    {'Z', {"#####", "   # ", "  #  ", " #   ", "#####"}},
    {'0', {" ### ", "#  ##", "# # #", "##  #", " ### "}},
    {'1', {"  #  ", " ##  ", "  #  ", "  #  ", " ### "}},
    {'2', {" ### ", "#   #", "  ## ", " #   ", "#####"}},
    {'3', {"#### ", "    #", " ### ", "    #", "#### "}},
    {'4', {"#   #", "#   #", "#####", "    #", "    #"}},
    {'5', {"#####", "#    ", "#### ", "    #", "#### "}},
    {'6', {" ### ", "#    ", "#### ", "#   #", " ### "}},
    {'7', {"#####", "    #", "   # ", "  #  ", "  #  "}},
    {'8', {" ### ", "#   #", " ### ", "#   #", " ### "}},
    {'9', {" ### ", "#   #", " ####", "    #", " ### "}},
    {' ', {"     ", "     ", "     ", "     ", "     "}},
    {'!', {"  #  ", "  #  ", "  #  ", "     ", "  #  "}},
    {'?', {" ### ", "#   #", "  ## ", "     ", "  #  "}},
    {'.', {"     ", "     ", "     ", "     ", "  #  "}},
    {',', {"     ", "     ", "     ", "  #  ", " #   "}},
    {':', {"     ", "  #  ", "     ", "  #  ", "     "}},
    {'-', {"     ", "     ", "#####", "     ", "     "}},
    {'+', {"     ", "  #  ", " ### ", "  #  ", "     "}}
};
#define FONT_COUNT (int)(sizeof(FONT) / sizeof(FONT[0]))

static const char* message = NULL;

void set_message(const char* text){
    message = text;
}

int has_message(void){
    return message != NULL && message[0] != '\0';
}

/* Unknown characters are shown as '?'. */
static const Glyph* find_glyph(char c){
    int i;
    char upper = (char)toupper((unsigned char)c);
    for(i = 0;i < FONT_COUNT;i++){
        if(FONT[i].c == upper){
            return &FONT[i];
        }
    }
    return find_glyph('?');
}

/* Takes as many whole words as fit in max characters, starting at start.
 * A single word longer than max is cut. Returns where the next chunk starts. */
static int split_chunk(const char* text,int start,int max,int* length){
    int total = (int)strlen(text);
    int cut;
    if(start + max >= total){
        *length = total - start;
        return total;
    }
    for(cut = start + max;cut > start && text[cut] != ' ';cut--);
    if(cut == start){
        cut = start + max;
    }
    *length = cut - start;
    while(text[cut] == ' '){
        cut++;
    }
    return cut;
}

/* Finds the n-th chunk (wrapping around) and returns its start. */
static int find_chunk(int n,int max,int* length){
    int count = 0, start = 0, next;
    while(start < (int)strlen(message)){
        start = split_chunk(message, start, max, length);
        count++;
    }
    n %= count;
    start = 0;
    while(1){
        next = split_chunk(message, start, max, length);
        if(n-- == 0){
            return start;
        }
        start = next;
    }
}

typedef struct {
    int top, left, center_x, center_y;
    int t, rise, reveal;
    int floor_row;
} Show;

/* Each pixel of the text appears once the explosion has spread to it,
 * twinkles while held, then falls and fades out. */
static void draw_pixel(const Show* s,int x,int y,int index){
    int since = s->t - s->rise;
    int fade = since - s->reveal - MESSAGE_HOLD;
    int dx = abs(x - s->center_x), dy = abs(y - s->center_y) * 2;
    unsigned int h = hash2(x * 7 + y, s->t);
    if((dx > dy ? dx : dy) > since * MESSAGE_REVEAL_SPEED){
        return;
    }
    if(fade > 0){
        if((int)(h % MESSAGE_FADE) < fade){
            return;
        }
        y += fade / 2;
    }
    if(y > s->floor_row){
        return;
    }
    attrset(COLOR_PAIR(index % 7 + 1) | A_BOLD);
    mvaddch(y, x, fade > 0 ? '.' : (h % 5 == 0 ? '+' : '*'));
}

static void draw_text(const Show* s,const char* text,int length){
    int i, row, col;
    const Glyph* g;
    for(i = 0;i < length;i++){
        g = find_glyph(text[i]);
        for(row = 0;row < FONT_ROWS;row++){
            for(col = 0;g->rows[row][col] != '\0';col++){
                if(g->rows[row][col] == '#'){
                    draw_pixel(s, s->left + i * GLYPH_WIDTH + col, s->top + row, i);
                }
            }
        }
    }
}

/* Shows the message as a text-shaped firework, repeating forever.
 * Long messages are split at spaces into chunks that fit the screen and shown in turn. */
void message_fireworks(int frame,int floor_row){
    Show s;
    int per_chunk = width / GLYPH_WIDTH;
    int start, length, period, y;

    if(!has_message() || per_chunk < 1 || floor_row < FONT_ROWS + 2){
        return;
    }
    while(*message == ' '){
        message++;
    }
    if(*message == '\0'){
        return;
    }

    s.floor_row = floor_row;
    s.top = (floor_row - FONT_ROWS) / 3;
    s.center_y = s.top + FONT_ROWS / 2;
    s.center_x = width / 2;
    s.rise = (floor_row - s.center_y) / MESSAGE_RISE_SPEED;
    s.reveal = width / 2 / MESSAGE_REVEAL_SPEED + 1;
    period = s.rise + s.reveal + MESSAGE_HOLD + MESSAGE_FADE + MESSAGE_REST;
    s.t = frame % period;

    start = find_chunk(frame / period, per_chunk, &length);
    s.left = (width - (length * GLYPH_WIDTH - 1)) / 2;

    if(s.t < s.rise){
        y = s.center_y + (s.rise - s.t) * MESSAGE_RISE_SPEED;
        attrset(COLOR_PAIR(7) | A_BOLD);
        mvaddch(y, s.center_x, 'o');
        if(y + 1 <= floor_row){
            mvaddch(y + 1, s.center_x, '|');
        }
    }else{
        draw_text(&s, message + start, length);
    }
    attrset(COLOR_PAIR(0));
}
