//**************************************************************************
//**
//** File: enemy.c (CyberSP Project)
//** Purpose: Enemies attributes
//** Last Update: 19-09-25 10:55
//** Author: DDeyTS
//**
//**************************************************************************

#include "enemy.h"
#include "bitmap.h"
#include "game.h"

SpawnEnemyData_t spawndata[]        = {{EN_GANGMEMBER, 300, 240},
                                       {EN_GANGMEMBER, 400, 300},
                                       {EN_GANGMEMBER, 450, 320},
                                       {EN_GANGMEMBER, 400, 200}};
int              num_spawn          = sizeof(spawndata) / sizeof(spawndata[0]);
int              num_active_enemies = 0;

//==========================================================================
//
//    InitEnemy
//
//    Argument: int slot        - which slots he is in the spawndata[] array
//              EnemiesID id    - what enemy is
//              float start_x   - initial position
//              float start_y
//    Return:   void
//
//    TODO: copy this function for NPC actors.
//
//==========================================================================

void InitEnemy(int slot, EnemiesID id, float start_x, float start_y)
{
    if (slot < 0 || slot >= MAX_ENEMIES) return;

    en[slot].px          = start_x;
    en[slot].py          = start_y;
    en[slot].fw          = 0;
    en[slot].fh          = 0;
    en[slot].speed       = 3;
    en[slot].frames      = 0.f;
    en[slot].reset_frame = 0;
    en[slot].dx          = 0;
    en[slot].dy          = 0;
    en[slot].move_count  = 0;
    en[slot].type        = id;
    en[slot].spr         = enemy_sprites[id];
}

//==========================================================================
//
//    SpawnAllEnemies
//
//    Argument: int n_enemies    - amount of enemies to appear
//    Return:   void
//
//==========================================================================

void SpawnAllEnemies(int n_enemies)
{
    num_active_enemies = n_enemies;
    if (num_active_enemies > num_spawn) num_active_enemies = num_spawn;
    if (num_active_enemies > MAX_ENEMIES) num_active_enemies = MAX_ENEMIES;

    for (int i = 0; i < num_active_enemies; i++) {
        InitEnemy(i, spawndata[i].id, spawndata[i].x, spawndata[i].y);
    }

    for (int i = num_active_enemies; i < MAX_ENEMIES; i++) {
        en[i].spr = NULL;
        en[i].dx = en[i].dy = 0;
        en[i].move_count    = 0;
    }
}

//==========================================================================
//
//    EnemyMovement
//
//    Argument: int e              - what enemy is
//              float *px, *py     - position
//              float sp           - speed
//              int *fx, *fy       - sprite sheet row and column
//              float frames       - amount of frames to animate
//              int dx, dy         - directions to follow
//    Return:   void
//
//    TODO: adjust this function for all the actors.
//
//==========================================================================

void EnemyMovement(int e, float *px, float *py, float sp, int *fx, int *fy,
                   float frames, int dx, int dy)
{
    int   cols = 16;
    int   rows = 24;
    float fq   = (cols * frames) + cols;

    if (dx > 0 && dy < 0) {
        *fx = fq, *fy = rows * 5;
        en[e].reset_frame = *fy;
    }
    else if (dx < 0 && dy < 0) {
        *fx = fq, *fy = rows * 6;
        en[e].reset_frame = *fy;
    }
    else if (dx > 0 && dy > 0) {
        *fx = fq, *fy = rows * 2;
        en[e].reset_frame = *fy;
    }
    else if (dx < 0 && dy > 0) {
        *fx = fq, *fy = rows;
        en[e].reset_frame = *fy;
    }
    else if (dx > 0) {
        *fx = fq, *fy = rows * 4;
        en[e].reset_frame = *fy;
    }
    else if (dx < 0) {
        *fx = fq, *fy = rows * 3;
        en[e].reset_frame = *fy;
    }
    else if (dy > 0) {
        *fx = fq, *fy = 0;
        en[e].reset_frame = *fy;
    }
    else if (dy < 0) {
        *fx = fq, *fy = (rows * 7) + 1;
        en[e].reset_frame = *fy;
    }
    else {
        *fx = 0;
        *fy = en[e].reset_frame;
    }

    float mov_x = dx * sp;
    float mov_y = dy * sp;

    if (dx != 0 && dy != 0) {
        float adj = 0.707f;
        mov_x *= adj;
        mov_y *= adj;
    }

    CollVSMove(px, py, mov_x, mov_y);
}

//==========================================================================
//
//    EnemyMoveAnim
//
//    Argument: int id    - what enemy is
//    Return:   void
//
//    TODO: adjust this function for all the actors.
//
//==========================================================================

void EnemyMoveAnim(int id)
{
    en[id].move_count++;
    if (en[id].move_count > 25) {
        en[id].move_count = 0;
        int r             = rand() % 4;
        switch (r) {
        case 0:
            en[id].dx = -1;
            en[id].dy = 0;
            break;
        case 1:
            en[id].dx = 1;
            en[id].dy = 0;
            break;
        case 2:
            en[id].dx = 0;
            en[id].dy = -1;
            break;
        case 3:
            en[id].dx = 0;
            en[id].dy = 1;
            break;
        }
    }

    en[id].frames += 0.3f;
    if (en[id].frames > 4) {
        en[id].frames -= 4;
    }
    EnemyMovement(id, &en[id].px, &en[id].py, en[id].speed, &en[id].fw, &en[id].fh,
                  (int)en[id].frames, en[id].dx, en[id].dy);
}

//==========================================================================
//
//    EnemyEncounter
//
//    Argument: int num_enemies    - how much the player has encountered
//    Return:   void
//
//==========================================================================

void EnemyEncounter(int num_enemies)
{
    for (int i = 0; i < num_enemies; i++) {
        if (en[i].spr) {
            EnemyMoveAnim(i);
        }
    }
}
