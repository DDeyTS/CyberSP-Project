#pragma once

#include "dialoguesys.h"

typedef struct {
  bool active;
  float x, start_y;
  float y;
  float anim_duration;
  float total_duration;
  float fly_distance;
  int dmg;
  ALLEGRO_COLOR color;
  char text[12];
} DamageNum_t;

void GetEnemyPos(int enemy_id, float *x, float *y);
int EnemyWasHit(int mx, int my);
void SpawnDamageNum(float x, float y, int damage);
void UpdateDamageNum(float dt);
void DrawDamageNum(ALLEGRO_FONT *font);
void ShootHit(float x, float y, int d20);

