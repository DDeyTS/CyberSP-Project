#ifndef BITMAP_H
#define BITMAP_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <math.h>
#include <stdio.h>

typedef enum {
  NONE_ENTITY = -1,
  ENTITY_GANGMEMBER,
  NUM_ENTITY,
} EntitiesID;

typedef struct {
  int fw, fh; /*rows, cols;*/ // frame manager
  float px, py;               // sprite movement
  float speed;
  float frames;
  int reset_frame;
  ALLEGRO_BITMAP *spr;
} SpriteSheetInfo;

void InitBitmap(void);
void DrawProtag(void);
void DrawEntity(void);
bool InitCursor(ALLEGRO_DISPLAY *disp);
void BitmapDraw(void);
void BitmapExplode(void);
void CursorChanger(void);

extern SpriteSheetInfo protag, ent[];
extern ALLEGRO_BITMAP *chatbox, *protagonist, *chatbox_light, *DBG_portrait;

#endif
