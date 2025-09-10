//**************************************************************************
//**
//** File: game.c (CyberSP Project)
//** Purpose: Game logic
//** Last Update: 08-09-2025 14:06
//** Author: DDeyTS
//**
//**************************************************************************

#include "game.h"
#include "bitmap.h"
#include "collision.h"
#include "debug.h"
#include "dialoguesys.h"
#include "input.h"
#include "main.h"
#include "tile_render.h"

static void ProtagMoveAnim();
static void EntityMoveAnim();

//==========================================================================
//
//    GameRedraw
//
//    Argument: void
//    Return:   void
//
//==========================================================================

void GameRedraw(void)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    RenderMap(map);

    DrawEveryStuff();

    //
    // Description Window
    //

    if (show_desc) {
        InitDescBox(desc[obj_desc]->pos_x, desc[obj_desc]->pos_y,
                    desc[obj_desc]->text);
    }

    //
    // Dialogue Window
    //
    {
        bool dlg_open   = (dlgstats.flags & DLG_OPEN) == DLG_OPEN;
        bool show_intro = (dlgstats.flags & SHOW_INTRO) == SHOW_INTRO;
        if (dlg_open) {
            if (show_intro) {
                InitDlgBox(npc[dlgstats.speaker]->portrait_id,
                           npc[dlgstats.speaker]->name,
                           npc[dlgstats.speaker]->topics->intro_text);
            }
            else if (active_topic >= 0) {
                const char *topic =
                    npc[dlgstats.speaker]->topics[selected_topic].topic;
                LoadDlg(npc[dlgstats.speaker], topic);
            }
            InitTopicMenu(npc[dlgstats.speaker], selected_topic);
        }
    }

    al_flip_display();
}

//==========================================================================
//
//    GameLoop
//
//    Argument: void
//    Return:   void
//
//==========================================================================

void GameLoop(void)
{
    al_wait_for_event(queue, &ev);

    KeyboardOn();
    MouseOn();

    CloseGame();

    // Main Timer
    if (ev.type == ALLEGRO_EVENT_TIMER) {
        MouseClick();

        ProtagMoveAnim();

        EntityMoveAnim();

        redraw = true;
    }

    if (redraw && al_is_event_queue_empty(queue)) {
        GameRedraw();
        redraw = false;
    }
}

//==========================================================================
//
//    GmaeCrusher
//
//    Argument: void
//    Return:   void
//
//==========================================================================

void GameCrusher(void)
{
    //
    // Dialogue Sys
    //

    al_destroy_bitmap(npc[dlgstats.speaker]->portrait_id);
    for (int i = 0; i < npc[dlgstats.speaker]->num_topic; i++) {
        free((char *)npc[dlgstats.speaker]->topics[i].topic);
        free((char *)npc[dlgstats.speaker]->topics[i].text);
    }
    free(npc[dlgstats.speaker]->topics);
    free(npc[dlgstats.speaker]);

    //
    // Description System
    //

    for (unsigned int i = 0; i < desc_count; i++) {
        free(desc[i]->text);
        free(desc[i]);
    }
    free(desc);

    tmx_map_free(map);

    ExplodeFont();

    BitmapExplode();

    if (cursors.normal) al_destroy_mouse_cursor(cursors.normal);
    if (cursors.view) al_destroy_mouse_cursor(cursors.view);
    if (cursors.aim) al_destroy_mouse_cursor(cursors.aim);
    if (cursors.clicking) al_destroy_mouse_cursor(cursors.clicking);

    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
    al_destroy_display(disp);
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
    // Pressed Directions
    //

    if (keys[ALLEGRO_KEY_D]) dx += 1;
    if (keys[ALLEGRO_KEY_A]) dx -= 1;
    if (keys[ALLEGRO_KEY_S]) dy += 1;
    if (keys[ALLEGRO_KEY_W]) dy -= 1;

    // apply speed
    float mov_x = dx * sp, mov_y = dy * sp;
    // adjust speed
    if (dx != 0 && dy != 0) {
        float adj = 0.707f;  // aka 1 / sqrt(2)
        mov_x *= adj;
        mov_y *= adj;
    }

    //
    // Collision Reader
    //
    // FIXME: sprite keeps getting into the wall collision
    {
        int colliders_count      = getColliderCount();
        CollisionRect *colliders = getColliders();
        int hitbox_w             = protag.fw;
        int hitbox_h             = protag.fh;

        float coll_px   = *px + mov_x;
        bool collided_x = false;
        for (int i = 0; i < colliders_count; i++) {
            if (RectSqColl(coll_px, *py, hitbox_w, hitbox_h, colliders[i].x,
                           colliders[i].y, colliders[i].w, colliders[i].h)) {
                collided_x = true;
                break;
            }
        }
        if (!collided_x) {
            *px = coll_px;
        }

        float coll_py   = *py + mov_y;
        bool collided_y = false;
        for (int i = 0; i < colliders_count; i++) {
            if (RectSqColl(*px, coll_py, hitbox_w, hitbox_h, colliders[i].x,
                           colliders[i].y, colliders[i].w, colliders[i].h)) {
                collided_y = true;
                break;
            }
        }
        if (!collided_y) {
            *py = coll_py;
        }
    }
}

//
//=============================
//
// ProtagMoveAnim
//
//=============================
//

void ProtagMoveAnim()
{
    protag.frames += 0.3f;    // frame speed
    if (protag.frames > 4) {  // reset frame queue
        protag.frames -= 4;
    }
    if (chosen_cursor == cursors.aim) {
        ToggleToAim();
    }
    else {
        ProtagMovement(keys, &protag.px, &protag.py, protag.speed, &protag.fw,
                       &protag.fh, (int)protag.frames);
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

//
//====================================
//
// EntityMoveAnim
//
// TODO: upgrade that into a function
//
//====================================
//

void EntityMoveAnim()
{
    static int move_count = 0;
    static int npc_x = 0, npc_y = 0;

    move_count++;
    if (move_count > 15) {
        move_count = 0;
        int r      = rand() % 4;
        switch (r) {
        case 0:
            npc_x = -1;
            npc_y = 0;
            break;
        case 1:
            npc_x = 1;
            npc_y = 0;
            break;
        case 2:
            npc_x = 0;
            npc_y = -1;
            break;
        case 3:
            npc_x = 0;
            npc_y = 1;
            break;
        }
    }

    int e = ENTITY_GANGMEMBER;
    ent[e].frames += 0.3f;
    if (ent[e].frames > 4) {
        ent[e].frames -= 4;
    }
    EntityMovement(e, &ent[e].px, &ent[e].py, ent[e].speed, &ent[e].fw, &ent[e].fh,
                   (int)ent[e].frames, npc_x, npc_y);
}
