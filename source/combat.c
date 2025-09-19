//**************************************************************************
//**
//** File: combat.c
//** Purpose: Combat system
//** Last Update: 17-09-25 13:45
//** Author: DDeyTS
//**
//**************************************************************************

#include "combat.h"
#include "bitmap.h"
#include "collision.h"
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

void GetEnemyPos(int enemy_id, float *x, float *y)
{
    *x = en[enemy_id].px;
    *y = en[enemy_id].py;
}

int EnemyWasHit(int mx, int my)
{
    for (int i = 0; i < NUM_ENEMY; i++) {
        float ex, ey;
        GetEnemyPos(i, &ex, &ey);
        if (mx >= ex && mx <= ex + (HITBOX_W - OFFSET_X) && my >= ey &&
            my <= ey + (HITBOX_H - OFFSET_Y)) {
            return i;
        }
    }
    return -1;
}

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
            dmgnum[i].active         = true;
            dmgnum[i].x              = x;
            dmgnum[i].start_y        = y;
            dmgnum[i].y              = y;
            dmgnum[i].dmg            = damage;
            dmgnum[i].total_duration = 1.0f;
            dmgnum[i].anim_duration  = dmgnum[i].total_duration;
            dmgnum[i].fly_distance   = 30.0f;
            float alpha =
                dmgnum[i].anim_duration / dmgnum[i].total_duration;  // fade out
            dmgnum[i].color = damage > 6 ? al_map_rgba(255, 0, 0, alpha)
                                         : al_map_rgba(255, 255, 255, alpha);
            snprintf(dmgnum[i].text, sizeof(dmgnum[i].text), "%d", damage);

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
        al_draw_text(font, dmgnum[i].color, dmgnum[i].x, dmgnum[i].y,
                     ALLEGRO_ALIGN_CENTRE, dmgnum[i].text);
    }
}

//==========================================================================
//
//    ShootHit
//
//    Argument: float x, y        - enemy position
//              int d20           - the result of the dice
//    Return:   void
//
//==========================================================================

void ShootHit(float x, float y, int d20)
{
    int dmg = RollD6();
    // FIXME: find a better way to read what enemy is hit.
    // x = en[EN_GANGMEMBER].px;
    // y = en[EN_GANGMEMBER].py;

    if (d20 == 1) {
        dmg *= 2;
        SpawnDamageNum(x, y, dmg);
        printf("Tiro atingiu em cheio! %d de dano crítico!\n", dmg);
    }
    else if (d20 >= 2 && d20 <= 10) {
        SpawnDamageNum(x, y, dmg);
        printf("Tiro atingiu o inimigo, infligindo %d de dano.\n", dmg);
    }
    else {
        printf("Tiro falhou!\n");
    }
}
