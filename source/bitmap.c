//**************************************************************************
//**
//** File: bitmap.c (CyberSP Project)
//** Purpose: Load graphic stuff
//**
//** Last Update: 16-09-2025 13:49
//** Author: DDeyTS
//**
//**************************************************************************

#include "bitmap.h"

// EXTERNAL DATA DECLARATIONS ///////////////////////////////////////////////

SpriteSheet_t protag        = {.fw          = 0,
                               .fh          = 0,
                               .px          = 320,
                               .py          = 200,
                               .speed       = 3.5,
                               .frames      = 0.f,
                               .reset_frame = 0};
SpriteSheet_t en[NUM_ENEMY] = {[EN_GANGMEMBER] = {.fw          = 0,
                                                  .fh          = 0,
                                                  .px          = 150,
                                                  .py          = 200,
                                                  .speed       = 3.5,
                                                  .frames      = 0.f,
                                                  .reset_frame = 0}};
ALLEGRO_BITMAP *chatbox, *protagonist, *chatbox_light, *DBG_portrait = NULL;

// PRIVATE DATA DEFINITIONS /////////////////////////////////////////////////
static DamageNum_t dmgnum[32];

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

    en[EN_GANGMEMBER].spr = al_load_bitmap("sprites/regis_spritesheet.png");

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
// DrawEN
//
// TODO: managing a lot of entities over here.
//
//======================
//

void DrawEnemy(void)
{
    // TODO: finding the best way to store every EN in the array
    al_draw_scaled_bitmap(en[EN_GANGMEMBER].spr, en[EN_GANGMEMBER].fw,
                          en[EN_GANGMEMBER].fh, 16, 24, en[EN_GANGMEMBER].px,
                          en[EN_GANGMEMBER].py, 32, 48, 0);
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
    DrawEnemy();
}

/* Damage numbers
 * in:
 * font, amount of damage, floating distance and limit, number position.
 * during:
 * when the debug key is pressed, a random number is
 * rolled for the damage value, which is put into print function together to
 * floating distance for the animation.
 * out:
 * little number floating above the target's head.
 *
 * FIXME: make it floating smoothly
 * */

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
//
// void DamageNumAnim(int damage)
// {
//     float x             = 100;
//     float y             = 150;
//     float fly_distance  = 30;
//     float speed         = 0.5;
//     ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);
//
//     char damage_str[10];
//     sprintf(damage_str, "%d", damage);
//
//     for (float offset = 0; offset < fly_distance; offset += speed) {
//         al_draw_text(font_std, color, x, y - offset, 0, damage_str);
//     }
//
//     fly_distance = -30;
// }

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
