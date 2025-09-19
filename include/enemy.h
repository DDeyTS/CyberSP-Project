#pragma once

#include "bitmap.h"
#include "main.h"

typedef struct {
  int id;
  float x, y;
} SpawnEnemyData_t;

void InitEnemy(int id, float start_x, float start_y);
void SpawnAllEnemies(void);

extern SpawnEnemyData_t spawndata[NUM_ENEMY];
