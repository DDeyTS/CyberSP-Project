#pragma once

#include "main.h"

#define MAX_ENEMIES 10

typedef enum {
  NONE_ENEMY = -1,
  EN_GANGMEMBER,
  NUM_ENEMY_ID,
} EnemiesID;

typedef struct {
  EnemiesID id;
  float x, y;
} SpawnEnemyData_t;

void InitEnemy(int slot, EnemiesID id, float start_x, float start_y);
void SpawnAllEnemies(int n_enemies);
void EnemyMovement(int e, float *px, float *py, float sp, int *fx, int *fy,
                   float frames, int dx, int dy);
void EnemyMoveAnim(int id);
void EnemyEncounter(int num_enemies);

extern SpawnEnemyData_t spawndata[];
extern int num_spawn;
extern int num_active_enemies;
