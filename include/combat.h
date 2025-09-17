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
  char text[12];
} DamageNum_t;

void SpawnDamageNum(float x, float y, int damage);
void UpdateDamageNum(float dt);
void DrawDamageNum(ALLEGRO_FONT *font);

