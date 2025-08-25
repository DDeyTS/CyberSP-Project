#ifndef INPUT_H
#define INPUT_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/events.h>
#include <allegro5/keyboard.h>

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    ALLEGRO_BITMAP *mouse_bmp, *target_bmp, *click_bmp, *eye_bmp;
    ALLEGRO_MOUSE_CURSOR *aim, *clicking, *normal, *view;
} Mousecursors;

enum CursorType {
    CURSOR_NORMAL,
    // CURSOR_CLICK,
    CURSOR_TARGET,
    CURSOR_EYE,
};

void KeyboardOn(void);
void MouseOn(void);
void MouseClick(void);
bool InitCursor(ALLEGRO_DISPLAY *disp);
void CloseGame(void);
void ToggleToAim(void);

extern Mousecursors cursors;
extern ALLEGRO_MOUSE_CURSOR *current_cursor;
extern enum CursorType cursor_flag;
extern ALLEGRO_MOUSE_CURSOR *chosen_cursor;

#endif
