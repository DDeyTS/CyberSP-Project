//**************************************************************************
//**
//** File: collision.c (CyberSP Project)
//** Purpose: Sprite interaction logic (triggers, walls)
//** Last Update: 03-09-2025 23:45
//** Author: DDeyTS
//**
//**************************************************************************

#include "collision.h"
#include "main.h"

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

static CollisionRect colliders[256];
static int colliders_count = 0;

//==========================================================================
//
//    RectSqColl
//
//    Argument: float px        - player/entity's current X position
//              float py        - player/entity's current Y position
//              int pw          - player/entity's width colision
//              int ph          - player/entity's width colision
//              float wx        - wall left horizontal edge
//              float wy        - wall left vertical edge
//              int ww          - wall right horizontal edge
//              int wh          - wall right vertical edge
//    Return:   bool
//
//==========================================================================

bool RectSqColl(float px, float py, int pw, int ph, float wx, float wy, int ww,
                int wh)
{
    return !(px + pw <= wx || px >= wx + ww || py + ph <= wy || py >= wy + wh);
}

//==========================================================================
//
//    CircleColl
//
//    Argument: float cx1       - player/entity's current X position
//              float cy1       - player/entity's current Y position
//              float r1        - player/entity's ray
//              float cx2       - circle's current X position
//              float cy2       - circle's current Y position
//              float r2        - circle's ray
//    Return:   bool
//
//==========================================================================

bool CircleColl(float cx1, float cy1, float r1, float cx2, float cy2, float r2)
{
    float dx   = cx1 - cx2;  // player_x + circle_x
    float dy   = cy1 - cy2;  // player_y + circle_y
    float rsum = r1 + r2;    // player_ray (aka frame_w) + circle_ray
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
    if (colliders_count < 256) {
        colliders[colliders_count].x = x;
        colliders[colliders_count].y = y;
        colliders[colliders_count].w = w;
        colliders[colliders_count].h = h;
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

int getColliderCount() {
  return colliders_count;
}

//
//====================================
//
// getColliders 
//
//====================================
// 

CollisionRect *getColliders() {
  return colliders;
}
