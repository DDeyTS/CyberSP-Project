//**************************************************************************
//**
//** File: bitmap.c (CyberSP Project)
//** Purpose: Sprite handling (animation, movement)
//**
//** Last Update: 02-09-2025 23:56
//** Author: DDeyTS
//**
//**************************************************************************

#include "bitmap.h"
#include "collision.h"
#include "dialoguesys.h"
#include "input.h"
#include "main.h"

// EXTERNAL DATA DECLARATIONS ///////////////////////////////////////////////

SpriteSheetInfo protag          = {.frame_w     = 0,
                                   .frame_h     = 0,
                                   .px          = 320,
                                   .py          = 200,
                                   .speed       = 3.5,
                                   .frames      = 0.f,
                                   .reset_frame = 0};
SpriteSheetInfo ent[NUM_ENTITY] = {[ENTITY_GANGMEMBER] = {.frame_w     = 0,
                                                          .frame_h     = 0,
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
    al_draw_scaled_bitmap(protag.spr, protag.frame_w, protag.frame_h, 16, 24,
                          protag.px, protag.py, 32, 48, 0);
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
    al_draw_scaled_bitmap(
        ent[ENTITY_GANGMEMBER].spr, ent[ENTITY_GANGMEMBER].frame_w,
        ent[ENTITY_GANGMEMBER].frame_h, 16, 24, ent[ENTITY_GANGMEMBER].px,
        ent[ENTITY_GANGMEMBER].py, 32, 48, 0);
}

//==========================================================================
//
//    ProtagMovement
//
//    Argument: bool keys[]        - read which keys are pressed
//              float *px          - protagonist's current X position
//              float *py          - protagonist's current Y position
//              float sp           - movement speed
//              int *fx            - sprite sheet's row to animate
//              int *fy            - sprite sheet's column to animate
//              float frames       - number of frames per second
//    Return:   void
//
//    TODO: another function to control entity's movement.
//    TODO: aligning the movement when another key is pressed at the same time.
//
//==========================================================================

void ProtagMovement(bool keys[], float *px, float *py, float sp, int *fx, int *fy,
                    float frames)
{
    int dx = 0, dy = 0;                 // current direction
    int cols = 16;                      // sprite sheet Y axis
    int rows = 24;                      // sprite sheet X axis
    float fq = (cols * frames) + cols;  // frame queue

    //
    // Diagonal Movement
    //

    if (keys[ALLEGRO_KEY_W] && keys[ALLEGRO_KEY_D]) {
        // Up-right
        *fx = fq, *fy = rows * 5;
        protag.reset_frame = *fy;
    }
    else if (keys[ALLEGRO_KEY_W] && keys[ALLEGRO_KEY_A]) {
        // Up-left
        *fx = fq, *fy = rows * 6;
        protag.reset_frame = *fy;
    }
    else if (keys[ALLEGRO_KEY_S] && keys[ALLEGRO_KEY_D]) {
        // Down-right
        *fx = fq, *fy = rows * 2;
        protag.reset_frame = *fy;
    }
    else if (keys[ALLEGRO_KEY_S] && keys[ALLEGRO_KEY_A]) {
        // Down-left
        *fx = fq, *fy = rows;
        protag.reset_frame = *fy;

        //
        // Straight Movement
        //
    }
    else if (keys[ALLEGRO_KEY_D]) {
        *fx = fq, *fy = rows * 4;
        protag.reset_frame = *fy;
    }
    else if (keys[ALLEGRO_KEY_A]) {
        *fx = fq, *fy = rows * 3;
        protag.reset_frame = *fy;
    }
    else if (keys[ALLEGRO_KEY_S]) {
        *fx = fq, *fy = 0;
        protag.reset_frame = *fy;
    }
    else if (keys[ALLEGRO_KEY_W]) {
        *fx = fq, *fy = (rows * 7) + 1;
        protag.reset_frame = *fy;
    }
    else {
        *fx = 0;
        *fy = protag.reset_frame;
    }

    //
    // Pressed directions
    //

    if (keys[ALLEGRO_KEY_D]) dx += 1;
    if (keys[ALLEGRO_KEY_A]) dx -= 1;
    if (keys[ALLEGRO_KEY_S]) dy += 1;
    if (keys[ALLEGRO_KEY_W]) dy -= 1;

    // Apply speed
    float mov_x = dx * sp, mov_y = dy * sp;
    // Adjust speed
    if (dx != 0 && dy != 0) {
        float adj = 0.707f;  // aka 1 / sqrt(2)
        mov_x *= adj;
        mov_y *= adj;
    }

    // applies adjusted movement values
    *px += mov_x;
    *py += mov_y;
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

//==========================================================================
//
//    CursorChanger
//
//    Argument: void
//    Return:   void
//
//==========================================================================

void CursorChanger(void)
{
    if (keys[ALLEGRO_KEY_T]) {          // NOTE: target mode
        chosen_cursor = cursors.aim;    // changes the cursor bitmap
        cursor_flag   = CURSOR_TARGET;  // flags the current cursor
        if (dlgstats.dlg_open)
            dlgstats.dlg_open = false;  // doesn't work during dialogue window
        if (!mouse_animating) {         // doesn't work during cursor click
            current_cursor = chosen_cursor;
            al_set_mouse_cursor(disp, current_cursor);
        }
    }
    else if (keys[ALLEGRO_KEY_H]) {  // NOTE: hand/normal mode
        chosen_cursor = cursors.normal;
        cursor_flag   = CURSOR_NORMAL;
        al_set_mouse_cursor(disp, current_cursor);
    }
    else if (keys[ALLEGRO_KEY_E]) {  // NOTE: eye/view mode
        chosen_cursor = cursors.view;
        cursor_flag   = CURSOR_EYE;
        if (dlgstats.dlg_open) dlgstats.dlg_open = false;
        if (!mouse_animating) {
            current_cursor = chosen_cursor;
            al_set_mouse_cursor(disp, current_cursor);
        }
    }
}

//
//===================================
//
// EntityMovement
//
//===================================
//

void EntityMovement(int e, float *px, float *py, float sp, int *fx, int *fy,
                    float frames, int dx, int dy)
{
    int cols = 16;
    int rows = 24;
    float fq = (cols * frames) + cols;

    if (dx > 0 && dy < 0) {
        *fx                = fq;
        *fy                = rows * 5;
        ent[e].reset_frame = *fy;
    }
    else if (dx < 0 && dy < 0) {
        *fx                = fq;
        *fy                = rows * 6;
        ent[e].reset_frame = *fy;
    }
    else if (dx > 0 && dy > 0) {
        *fx                = fq;
        *fy                = rows * 2;
        ent[e].reset_frame = *fy;
    }
    else if (dx < 0 && dy > 0) {
        *fx                = fq;
        *fy                = rows;
        ent[e].reset_frame = *fy;
    }
    else if (dx > 0) {
        *fx                = fq;
        *fy                = rows * 4;
        ent[e].reset_frame = *fy;
    }
    else if (dx < 0) {
        *fx                = fq;
        *fy                = rows * 3;
        ent[e].reset_frame = *fy;
    }
    else if (dy > 0) {
        *fx                = fq;
        *fy                = 0;
        ent[e].reset_frame = *fy;
    }
    else if (dy < 0) {
        *fx                = fq;
        *fy                = (rows * 7) + 1;
        ent[e].reset_frame = *fy;
    }
    else {
        *fx = 0;
        *fy = ent[e].reset_frame;
    }

    float mov_x = dx * sp;
    float mov_y = dy * sp;

    if (dx != 0 && dy != 0) {
        float adj = 0.707f;
        mov_x *= adj;
        mov_y *= adj;
    }

    *px += mov_x;
    *py += mov_y;
}
