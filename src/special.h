#ifndef SPECIAL_H
#define SPECIAL_H

#define SHAPE_NONE -1

int pick_shape(void);
void draw_shape(int shape,int time,int pos,int center);
void start_starmine(void);
void start_big(int floor_row);
void special_fireworks(int floor_row);

#endif
