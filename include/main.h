#ifndef MAIN_H
#define MAIN_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/events.h>
#include <allegro5/keyboard.h>

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include <tmx.h>

#define DISPW 640
#define DISPH 400
#define MOUSE_MAX 5

extern tmx_map *map;

extern ALLEGRO_EVENT ev;
extern ALLEGRO_DISPLAY *disp;
extern ALLEGRO_TIMER *timer;
extern ALLEGRO_EVENT_QUEUE *queue;

extern bool keys[ALLEGRO_KEY_MAX], mouse[MOUSE_MAX + 1];
extern int mouse_x, mouse_y;
extern bool mouse_animating;

extern int obj_desc;
extern bool show_desc;

extern bool show_intro, dlg_open, choosing_topic;
extern int speaker;
extern int selected_topic;
extern int active_topic;

extern float sp;
extern float frames;

extern bool running;
extern bool redraw;

#endif
