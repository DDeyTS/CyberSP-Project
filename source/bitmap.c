//**************************************************************************
//**
//** File: bitmap.c (CyberSP Project)
//** Purpose: Load graphic stuff
//**
//** Last Update: 19-09-2025 12:38
//** Author: DDeyTS
//**
//**************************************************************************

#include "bitmap.h"
#include "collision.h"
#include "enemy.h"
#include "input.h"
#include "main.h"
#include "tile_render.h"

// EXTERNAL DATA DECLARATIONS ///////////////////////////////////////////////

SpriteInfo_t    protag = {.fw          = 0,
                          .fh          = 0,
                          .px          = 320,
                          .py          = 200,
                          .speed       = 3.5,
                          .frames      = 0.f,
                          .reset_frame = 0};

SpriteInfo_t    en[MAX_ENEMIES];
ALLEGRO_BITMAP *enemy_sprites[NUM_ENEMY_ID] = {NULL};

ALLEGRO_BITMAP *chatbox, *protagonist, *chatbox_light, *DBG_portrait = NULL;

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
    if (!protag.spr) {
        perror("Fail to load protagonist sprite!\n");
        exit(1);
    }
    if (!protagonist) {
        perror("Fail to load protagonist's face. Replacing it!\n");
        goto placeholder;
    }

    for (int i = 0; i < num_spawn; i++) {
        enemy_sprites[i] = al_load_bitmap("sprites/regis_spritesheet.png");
        if (!enemy_sprites[i]) {
            perror("Fail to load enemy sprite!\n");
            exit(1);
        }
    }

    chatbox       = al_load_bitmap("sprites/dlgbox_sprite.png");
    chatbox_light = al_load_bitmap("sprites/signal_light_chatbox_spritesheet.png");
    DBG_portrait  = al_load_bitmap("portraits/balldebug_face.png");
    if (!chatbox || !chatbox_light) {
        perror("Fail to load chatbox\n");
        exit(1);
    }

    cursors.mouse_bmp  = al_load_bitmap("sprites/handcursor_sprite.png");
    cursors.click_bmp  = al_load_bitmap("sprites/handcursor_click_sprite.png");
    cursors.target_bmp = al_load_bitmap("sprites/target_sprite.png");
    cursors.eye_bmp    = al_load_bitmap("sprites/eye_cursor.png");
    if (!cursors.eye_bmp || !cursors.mouse_bmp || !cursors.click_bmp ||
        !cursors.target_bmp) {
        perror("Fail to load mouse cursor!\n");
        exit(1);
    }

placeholder:
    if (!protagonist) protagonist = DBG_portrait;
}

//==========================================================================
//
//    DrawProtag
//
//    Argument: void
//    Return:   void
//
//    NOTE: the sprite is scaled, so take care of remembering to adapt
//    the code for its new size.
//
//==========================================================================

void DrawProtag(void)
{
    al_draw_scaled_bitmap(protag.spr, protag.fw, protag.fh, FRAME_W, FRAME_H, protag.px,
                          protag.py, HITBOX_W, HITBOX_H, 0);
}

//==========================================================================
//
//    DrawEnemy
//
//    Argument: void
//    Return:   void
//
//    NOTE: the sprite is scaled, so take care of remembering to adapt
//    the code for its new size.
//
//==========================================================================

void DrawEnemies(void)
{
    for (int i = 0; i < num_active_enemies; i++) {
        if (!en[i].spr) continue;
        al_draw_scaled_bitmap(en[i].spr, en[i].fw, en[i].fh, FRAME_W, FRAME_H, en[i].px,
                              en[i].py, HITBOX_W, HITBOX_H, 0);
    }
}

//==========================================================================
//
//    InitCursor
//
//    Argument: ALLEGRO_DISPLAY *disp    - linker for cursor to the display
//    Return:   bool
//
//==========================================================================

bool InitCursor(ALLEGRO_DISPLAY *disp)
{
    if (!cursors.eye_bmp || !cursors.mouse_bmp || !cursors.click_bmp ||
        !cursors.target_bmp)
        goto error;

    cursors.normal   = al_create_mouse_cursor(cursors.mouse_bmp, 0, 0);
    cursors.clicking = al_create_mouse_cursor(cursors.click_bmp, 0, 0);
    cursors.aim      = al_create_mouse_cursor(cursors.target_bmp, 0, 0);
    cursors.view     = al_create_mouse_cursor(cursors.eye_bmp, 0, 0);
    if (!cursors.normal || !cursors.clicking || !cursors.aim || !cursors.view)
        goto error;

    chosen_cursor  = cursors.normal;
    cursor_flag    = CURSOR_NORMAL;
    current_cursor = chosen_cursor;
    al_set_mouse_cursor(disp, current_cursor);
    al_show_mouse_cursor(disp);

    return true;

error:
    perror("Fail to load cursor bitmap or create mouse cursor!\n");
    return false;
}

//==========================================================================
//
//    BitmapDraw
//
//    Argument: void
//    Return:   void
//
//==========================================================================

void BitmapDraw()
{
    RenderMap(map);
    DrawProtag();
    DrawEnemies();
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
    al_destroy_bitmap(en[EN_GANGMEMBER].spr);
    al_destroy_bitmap(chatbox);
    al_destroy_bitmap(chatbox_light);
    al_destroy_bitmap(protagonist);
    if (cursors.mouse_bmp) al_destroy_bitmap(cursors.mouse_bmp);
    if (cursors.click_bmp) al_destroy_bitmap(cursors.click_bmp);
    if (cursors.target_bmp) al_destroy_bitmap(cursors.target_bmp);
    if (cursors.eye_bmp) al_destroy_bitmap(cursors.eye_bmp);
}
