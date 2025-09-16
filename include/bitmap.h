#pragma once

#include "collision.h"
#include "dialoguesys.h"
#include "input.h"
#include "main.h"
#include "tile_render.h"
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

typedef struct {
  bool active;
  float x, start_y;
  float y;
  float anim_duration;
  float total_duration;
  float fly_distance;
  int dmg;
  char text[12];
} DamageNum_t;

void InitBitmap(void);
void DrawProtag(void);
void DrawEnemy(void);
bool InitCursor(ALLEGRO_DISPLAY *disp);
void BitmapDraw(void);
void SpawnDamageNum(float x, float y, int damage);
void UpdateDamageNum(float dt);
void DrawDamageNum(ALLEGRO_FONT *font);
void BitmapExplode(void);
void CursorChanger(void);

extern SpriteSheet_t protag, en[];
extern ALLEGRO_BITMAP *chatbox, *protagonist, *chatbox_light, *DBG_portrait;
