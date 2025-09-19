#pragma once

#include "main.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <math.h>
#include <stdbool.h>

#define HITBOX_W 32
#define HITBOX_H 48
#define OFFSET_X 4
#define OFFSET_Y 8

typedef struct {
  float x, y;
  float w, h;
  float vx, vy; // future speed (optional)
} AABB2D_t;

void AABBInit(AABB2D_t *aabb, float x, float y, float w, float h);
int AABBCollides(AABB2D_t *a, AABB2D_t *b);
int AABBCollidesWithPoint(AABB2D_t *a, float px, float py);
bool CircleColl(float cx1, float cy1, float r1, float cx2, float cy2, float r2);
void AddCollRect(float x, float y, int w, int h);
int getColliderCount();
AABB2D_t *getColliders();
void CollVSMove(float *px, float *py, float mov_x, float mov_y);
