#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <math.h>
#include <stdio.h>

#define MAX_DMG_NUM 32

typedef enum {
  NONE_ENEMY = -1,
  EN_GANGMEMBER,
  NUM_ENEMY,
} EnemiesID;

typedef struct {
  int fw, fh; /*rows, cols;*/ // frame manager
  float px, py;               // sprite movement
  float speed;
  float frames;
  int reset_frame;
  ALLEGRO_BITMAP *spr;
} SpriteSheet_t;

void InitBitmap(void);
void DrawProtag(void);
void DrawEnemy(void);
bool InitCursor(ALLEGRO_DISPLAY *disp);
void BitmapDraw(void);
void BitmapExplode(void);
void CursorChanger(void);

extern SpriteSheet_t protag, en[];
extern ALLEGRO_BITMAP *chatbox, *protagonist, *chatbox_light, *DBG_portrait;
