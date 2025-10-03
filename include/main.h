#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/events.h>
#include <allegro5/keyboard.h>

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include <tmx.h>

#define DISPW 640
#define DISPH 480
#define MOUSE_MAX 5

extern tmx_map *map;

extern ALLEGRO_EVENT ev;
extern ALLEGRO_DISPLAY *disp;
extern ALLEGRO_TIMER *timer;
extern ALLEGRO_EVENT_QUEUE *queue;
extern ALLEGRO_AUDIO_STREAM *bgm, *dlg_ost, *tense_ost;

extern float dt;

extern bool keys[ALLEGRO_KEY_MAX], mouse[MOUSE_MAX + 1];
extern int mouse_x, mouse_y;
extern bool mouse_animating;

extern bool show_desc;

extern int selected_topic;
extern int active_topic;

extern bool running;
extern bool redraw;
