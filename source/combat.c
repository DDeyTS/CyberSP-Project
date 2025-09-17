//**************************************************************************
//**
//** File: combat.c
//** Purpose: Combat system
//** Last Update: 17-09-25 13:45
//** Author: DDeyTS
//**
//**************************************************************************

#include "combat.h"
#include "dice.h"
#include "input.h"

// EXTERNAL FUNCTION PROTOTYPES ////////////////////////////////////////////

// PUBLIC FUNCTION PROTOYPES ///////////////////////////////////////////////

// PRIVATE FUNCTION PROTOTYPES /////////////////////////////////////////////

// EXTERNAL DATA DECLARATIONS //////////////////////////////////////////////

// PUBLIC DATA DEFINITIONS /////////////////////////////////////////////////

// PRIVATE DATA DEFINITIONS ////////////////////////////////////////////////
static DamageNum_t dmgnum[32];

// CODE ////////////////////////////////////////////////////////////////////

//==========================================================================
//
//    SpawnDamageNum
//
//    Argument: float x, y        - position to draw 
//              int damage        - amount of hit points
//    Return:   void
//
//==========================================================================

void SpawnDamageNum(float x, float y, int damage)
{
    for (int i = 0; i < MAX_DMG_NUM; i++) {
        if (!dmgnum[i].active) {
            dmgnum[i].active  = true;
            dmgnum[i].x       = x;
            dmgnum[i].start_y = y;
            dmgnum[i].y       = y;
            dmgnum[i].dmg     = damage;
            snprintf(dmgnum[i].text, sizeof(dmgnum[i].text), "%d", damage);
            dmgnum[i].total_duration = 1.0f;
            dmgnum[i].anim_duration  = dmgnum[i].total_duration;
            dmgnum[i].fly_distance   = 30.0f;
            return;
        }
    }
}

//==========================================================================
//
//    UpdateDamageNum
//
//    Argument: float dt        - delta time 
//    Return:   void
//
//==========================================================================

void UpdateDamageNum(float dt)
{
    for (int i = 0; i < MAX_DMG_NUM; i++) {
        if (!dmgnum[i].active) continue;

        dmgnum[i].anim_duration -= dt;
        if (dmgnum[i].anim_duration <= 0.0f) {
            dmgnum[i].active = false;
            continue;
        }

        // smooth flight
        float t     = 1.0f - (dmgnum[i].anim_duration / dmgnum[i].total_duration);
        float ease  = 1.0f - powf(1.0f - t, 3.0f);
        dmgnum[i].y = dmgnum[i].start_y - dmgnum[i].fly_distance * ease;
    }
}

//==========================================================================
//
//    DrawDamageNum
//
//    Argument: ALLEGRO_FONT *font        - .ttf font to use 
//    Return:   void
//
//==========================================================================

void DrawDamageNum(ALLEGRO_FONT *font)
{
    for (int i = 0; i < MAX_DMG_NUM; i++) {
        if (!dmgnum[i].active) continue;
        float alpha =
            dmgnum[i].anim_duration / dmgnum[i].total_duration;  // fade out
        ALLEGRO_COLOR color = al_map_rgba_f(1.0f, 1.0f, 1.0f, alpha);
        al_draw_text(font, color, dmgnum[i].x, dmgnum[i].y, ALLEGRO_ALIGN_CENTRE,
                     dmgnum[i].text);
    }
}
