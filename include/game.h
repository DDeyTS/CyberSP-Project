#ifndef GAME_H
#define GAME_H

#include "main.h"

void GameRedraw(void);
void GameCrusher(void);
void GameLoop(void);
void ProtagMovement(bool keys[], float *px, float *py, float sp, int *fx,
                    int *fy, float frames);
void SpriteAimAtCursor(float px, float py, int *fy);
void EntityMovement(int e, float *px, float *py, float sp, int *fx, int *fy,
                    float frames, int dx, int dy);

#endif
