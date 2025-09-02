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
  int frame_w, frame_h, rows, cols; // frame manager
  float px, py;                     // sprite movement
  ALLEGRO_BITMAP *spr;
  float frames;
  int reset_frame;
} SpriteSheetInfo;

extern SpriteSheetInfo protag, ent[];

void InitBitmap(void);
void BitmapExplode(void);
void DrawProtag(void);
void DrawEntity(void);
void ProtagMovement(bool keys[], float *px, float *py, float sp, int *fx,
                    int *fy, float frames);
void SpriteAimAtCursor(float px, float py, int *fy);
void CursorChanger(void);
void EntityMovement(int e, float *px, float *py, float sp, int *fx, int *fy,
                    float frames, int dx, int dy);

extern ALLEGRO_BITMAP *chatbox, *protagonist, *chatbox_light;

#endif
