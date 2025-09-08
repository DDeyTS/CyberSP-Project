#ifndef COLLISION_H
#define COLLISION_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <math.h>
#include <stdbool.h>

typedef struct {
  float x, y;
  int w, h;
} CollisionRect;

bool RectSqColl(float px, float py, int pw, int ph, float wall_x1,
                float wall_y1, int wall_x2, int wall_y2);
bool CircleColl(float cx1, float cy1, float r1, float cx2, float cy2, float r2);
void AddCollRect(float x, float y, int w, int h);
// void CollDetect(float *px, float *py, float coll_px, float coll_py,
                // float hitbox_w, float hitbox_h);

int getColliderCount();
CollisionRect *getColliders();

#endif
