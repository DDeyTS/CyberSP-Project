#pragma once

#include "bitmap.h"
#include "collision.h"
#include "debug.h"
#include "dialoguesys.h"
#include "input.h"
#include "main.h"
#include "tile_render.h"

#define EPSILON 0.1f

void GameRedraw(void);
void GameCrusher(void);
void GameLoop(void);
void ProtagMovement(bool keys[], float *px, float *py, float sp, int *fx,
                    int *fy, float frames);
void SpriteAimAtCursor(float px, float py, int *fy);
void EnemyMovement(int e, float *px, float *py, float sp, int *fx, int *fy,
                   float frames, int dx, int dy);
