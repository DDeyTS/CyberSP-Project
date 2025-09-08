//**************************************************************************
//**
//** File: bitmap.c (CyberSP Project)
//** Purpose: Sprite handling (animation, movement)
//**
//** Last Update: 08-09-2025 13:59
//** Author: DDeyTS
//**
//**************************************************************************

#include "bitmap.h"
#include "collision.h"
#include "dialoguesys.h"
#include "input.h"
#include "main.h"

// EXTERNAL DATA DECLARATIONS ///////////////////////////////////////////////

SpriteSheetInfo protag          = {.fw          = 0,
                                   .fh          = 0,
                                   .px          = 320,
                                   .py          = 200,
                                   .speed       = 3.5,
                                   .frames      = 0.f,
                                   .reset_frame = 0};
SpriteSheetInfo ent[NUM_ENTITY] = {[ENTITY_GANGMEMBER] = {.fw          = 0,
                                                          .fh          = 0,
                                                          .px          = 150,
                                                          .py          = 200,
                                                          .speed       = 3.5,
                                                          .frames      = 0.f,
                                                          .reset_frame = 0}};
ALLEGRO_BITMAP *chatbox, *protagonist, *chatbox_light = NULL;

// PRIVATE DATA DEFINITIONS /////////////////////////////////////////////////

//==========================================================================
//
//    InitBitmap
//
//    Argument: void
//    Return:   void
//
//    Note: this function loads all the sprites.
//
//==========================================================================

void InitBitmap(void)
{
    protag.spr  = al_load_bitmap("sprites/regis_spritesheet.png");
    protagonist = al_load_bitmap("portraits/regis_face.png");
    if (!protag.spr || !protagonist) {
        perror("Fail to load protagonist bitmap!\n");
        exit(1);
    }

    // sprite to debug entity render
    ent[ENTITY_GANGMEMBER].spr = al_load_bitmap("sprites/regis_spritesheet.png");

    chatbox       = al_load_bitmap("sprites/chatbox_sprite.png");
    chatbox_light = al_load_bitmap("sprites/signal_light_chatbox_spritesheet.png");
    if (!chatbox || !chatbox_light) {
        perror("Fail to load chatbox bitmap!\n");
        exit(1);
    }

    cursors.mouse_bmp  = al_load_bitmap("sprites/handcursor_sprite.png");
    cursors.click_bmp  = al_load_bitmap("sprites/handcursor_click_sprite.png");
    cursors.target_bmp = al_load_bitmap("sprites/target_sprite.png");
    cursors.eye_bmp    = al_load_bitmap("sprites/eye_cursor.png");
    if (!cursors.eye_bmp || !cursors.mouse_bmp || !cursors.click_bmp ||
        !cursors.target_bmp) {
        perror("Fail to load cursor bitmap\n");
        exit(1);
    }
}

//==========================================================================
//
//    DrawProtag
//
//    Argument: void
//    Return:   void
//
//    NOTE: this function draws the protagonist's sprite.
//    NOTE: the sprite is scaled, so take care of remembering to adapt
//    the code for its new size.
//
//==========================================================================

void DrawProtag(void)
{
    al_draw_scaled_bitmap(protag.spr, protag.fw, protag.fh, 16, 24, protag.px,
                          protag.py, 32, 48, 0);
}

//
//======================
//
// DrawEntity
//
// TODO: managing a lot of entities over here.
//
//======================
//

void DrawEntity(void)
{
    // TODO: finding the best way to store every entity in the array
    al_draw_scaled_bitmap(ent[ENTITY_GANGMEMBER].spr, ent[ENTITY_GANGMEMBER].fw,
                          ent[ENTITY_GANGMEMBER].fh, 16, 24,
                          ent[ENTITY_GANGMEMBER].px, ent[ENTITY_GANGMEMBER].py, 32,
                          48, 0);
}

void DrawEveryStuff()
{
    DrawProtag();
    DrawEntity();
}

//==========================================================================
//
//    BitmapExplode
//
//    Argument: void
//    Return:   void
//
//    NOTE: this function destroys every sprite when the game is closed!!!
//
//==========================================================================

void BitmapExplode(void)
{
    al_destroy_bitmap(protag.spr);
    al_destroy_bitmap(ent[ENTITY_GANGMEMBER].spr);
    al_destroy_bitmap(chatbox);
    al_destroy_bitmap(chatbox_light);
    al_destroy_bitmap(protagonist);
    if (cursors.mouse_bmp) al_destroy_bitmap(cursors.mouse_bmp);
    if (cursors.click_bmp) al_destroy_bitmap(cursors.click_bmp);
    if (cursors.target_bmp) al_destroy_bitmap(cursors.target_bmp);
    if (cursors.eye_bmp) al_destroy_bitmap(cursors.eye_bmp);
}

//==========================================================================
//
//    SpriteAimAtCursor
//
//    Argument: float px        - current sprite's X position
//              float py        - current sprite's Y position
//              float *fy       - sprite sheet's column to animate
//    Return:   void
//
//    NOTE: ignores both px and py warning below.
//
//==========================================================================

void SpriteAimAtCursor(float px, float py, int *fy)
{
    float t_dx    = mouse_x - (protag.px + 16);  // sprite center
    float t_dy    = mouse_y - (protag.py + 24);  // same above
    float t_angle = atan2(t_dy, t_dx);           // radianus (-PI to +PI)

    int dir;
    // right
    if (t_angle >= -ALLEGRO_PI / 8 && t_angle < ALLEGRO_PI / 8) dir = 4;
    // down-right
    else if (t_angle >= ALLEGRO_PI / 8 && t_angle < 3 * ALLEGRO_PI / 8)
        dir = 2;
    // down
    else if (t_angle >= 3 * ALLEGRO_PI / 8 && t_angle < 5 * ALLEGRO_PI / 8)
        dir = 0;
    // down-left
    else if (t_angle >= 5 * ALLEGRO_PI / 8 && t_angle < 7 * ALLEGRO_PI / 8)
        dir = 1;
    // left
    else if (t_angle >= 7 * ALLEGRO_PI / 8 || t_angle < -7 * ALLEGRO_PI / 8)
        dir = 3;
    // up-left
    else if (t_angle >= -7 * ALLEGRO_PI / 8 && t_angle < -5 * ALLEGRO_PI / 8)
        dir = 6;
    // up
    else if (t_angle >= -5 * ALLEGRO_PI / 8 && t_angle < -3 * ALLEGRO_PI / 8)
        dir = 7;
    // up-right
    else
        dir = 5;

    // applies current rotation to animate
    *fy = dir * 24;
}
