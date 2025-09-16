#ifndef COLLISION_H
#define COLLISION_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <math.h>
#include <stdbool.h>

typedef struct {
    float x, y;
    float w, h;
    float vx, vy; // future speed
} AABB2D_t;

// typedef struct {
//   float x, y;
//   int w, h;
// } CollisionRect;

void AABBInit(AABB2D_t *aabb, float x, float y, float w, float h);
int AABBCollides(AABB2D_t *a, AABB2D_t *b);
int AABBCollidesWithPoint(AABB2D_t *a, float px, float py);
// bool RectSqColl(float ax, float ay, int aw, int ah, float bx,
//                float by, int bw, int bh);
bool CircleColl(float cx1, float cy1, float r1, float cx2, float cy2, float r2);
void AddCollRect(float x, float y, int w, int h);
// void CollDetect(float *px, float *py, float coll_px, float coll_py,
                // float hitbox_w, float hitbox_h);

int getColliderCount();
AABB2D_t *getColliders();

#endif
