//**************************************************************************
//**
//** File: collision.c (CyberSP Project)
//** Purpose: Sprite interaction logic (triggers, walls)
//** Last Update: 16-09-2025 14:30
//** Author: DDeyTS
//**
//**************************************************************************

#include "collision.h"

/*
      Radical change!

      My personal objective to develop an isometric game
      as a first project is far away due to my programming
      skills. Then I'm following my brother's advice of
      working with a 2D common point-of-view. It'll be simple
      enough to build something that I can later be proud of.
      The focus now is acquiring quite gamedev knowledge because,
      first of all, I'm in that only for personal achieviement.
        Well, let's build a top-down cyberpunk game with
      a lot of CRPG inspirations!
                                            (July 13, 2025)
*/

static AABB2D_t colliders[100];
static int      colliders_count = 0;

void AABBInit(AABB2D_t *aabb, float x, float y, float w, float h)
{
    aabb->x  = x;
    aabb->y  = y;
    aabb->w  = w;
    aabb->h  = h;
    aabb->vx = 0;
    aabb->vy = 0;
}

int AABBCollides(AABB2D_t *a, AABB2D_t *b)
{
    return (a->x < b->x + b->w) && (a->x + a->w > b->x) && (a->y < b->y + b->h) &&
           (a->y + a->h > b->y);
}

int AABBCollidesWithPoint(AABB2D_t *a, float px, float py)
{
    return (px >= a->x && px <= a->x + a->w && py >= a->y && py <= a->y + a->h);
}

// //==========================================================================
// //
// //    RectSqColl
// //
// //    Argument: float px, py    - actor's current X and Y position
// //              int pw, ph      - actor's width and height colision
// //              float wx        - wall left horizontal edge
// //              float wy        - wall left vertical edge
// //              int ww          - wall right horizontal edge
// //              int wh          - wall right vertical edge
// //    Return:   bool
// //
// //==========================================================================
//
// bool RectSqColl(float ax, float ay, int aw, int ah, float bx, float by, int bw,
//                 int bh)
// {
//     // return (ax < bx + bw) && (ax + aw > bx) && (ay < by + bh) && (ay + ah >
//     by); return !(ax + aw <= bx || ax >= bx + bw || ay + ah <= by || ay >= by +
//     bh);
// }

//==========================================================================
//
//    CircleColl
//
//    Argument: float px, py    - actor's current X and Y position
//              float r1        - player/entity's ray
//              float cx, cy    - circle's current X and Y position
//              float r2        - circle's ray
//    Return:   bool
//
//==========================================================================

bool CircleColl(float px, float py, float r1, float cx, float cy, float r2)
{
    float dx   = px - cx;
    float dy   = py - cy;
    float rsum = r1 + r2;
    return (dx * dx + dy * dy) < (rsum * rsum);
}

//
//====================================
//
// AddCollRect
//
//====================================
//

void AddCollRect(float x, float y, int w, int h)
{
    if (colliders_count < 100) {
        AABBInit(&colliders[colliders_count], x, y, w, h);
        colliders_count++;
    }
}

//
//====================================
//
// getColliderCount
//
//====================================
//

int getColliderCount() { return colliders_count; }

//
//====================================
//
// getColliders
//
//====================================
//

AABB2D_t *getColliders() { return colliders; }

//
//====================================
//
// CollisionVSMovement
//
//====================================
//

void CollVSMove(float *px, float *py, float mov_x, float mov_y)
{
    float     next_px         = *px + mov_x;
    float     next_py         = *py + mov_y;
    int       colliders_count = getColliderCount();
    AABB2D_t *colliders       = getColliders();

    // X Axis
    AABB2D_t nextX;
    AABBInit(&nextX, next_px, *py, HITBOX_W, HITBOX_H);
    bool collide_x = false;
    for (int i = 0; i < colliders_count; i++) {
        if (AABBCollides(&nextX, &colliders[i])) {
            collide_x = true;
            break;
        }
    }
    if (!collide_x) *px = next_px;

    // Y Axis
    AABB2D_t nextY;
    AABBInit(&nextY, *px, next_py, HITBOX_W, HITBOX_H);
    bool collide_y = false;
    for (int i = 0; i < colliders_count; i++) {
        if (AABBCollides(&nextY, &colliders[i])) {
            collide_y = true;
            break;
        }
    }
    if (!collide_y) *py = next_py;
}
