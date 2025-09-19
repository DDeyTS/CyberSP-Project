#pragma once

#include "enemy.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <math.h>
#include <stdio.h>

#define FRAME_W 16
#define FRAME_H 24

typedef struct {
  int fw, fh;   // frame manager
  float px, py; // sprite movement
  float speed;
  float frames;
  int reset_frame;
  ALLEGRO_BITMAP *spr;

  int dx, dy;     // enemies directions
  int move_count; // for the random movement
  EnemiesID type; // memorizes the actual enemy
} SpriteInfo_t;

void InitBitmap(void);
void DrawProtag(void);
void DrawEnemies(void);
bool InitCursor(ALLEGRO_DISPLAY *disp);
void BitmapDraw(void);
void BitmapExplode(void);
void CursorChanger(void);

extern SpriteInfo_t protag, en[MAX_ENEMIES];
extern ALLEGRO_BITMAP *enemy_sprites[NUM_ENEMY_ID];
extern ALLEGRO_BITMAP *chatbox, *protagonist, *chatbox_light, *DBG_portrait;
