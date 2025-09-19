//**************************************************************************
//**
//** File: game.c (CyberSP Project)
//** Purpose: Game logic
//** Last Update: 18-09-2025 22:51
//** Author: DDeyTS
//**
//**************************************************************************

#include "game.h"
#include "bitmap.h"
#include "collision.h"
#include "combat.h"
#include "dialoguesys.h"
#include "enemy.h"

static void ProtagMoveAnim();

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

    BitmapDraw();

    InitTextBoxes();

    DrawDamageNum(font_std);

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

    UpdateDamageNum(dt);

    CloseGame();

    // Main Timer
    if (ev.type == ALLEGRO_EVENT_TIMER) {
        MouseClick();

        ProtagMoveAnim();
        EnemyEncounter(num_spawn);

        redraw = true;
    }

    if (redraw && al_is_event_queue_empty(queue)) {
        GameRedraw();
        redraw = false;
    }
}

//==========================================================================
//
//    GameCrusher
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
//              float *px, *py     - current either X and Y position
//              float sp           - movement speed
//              int *fx, *fy       - sprite sheet's row and column to animate
//              float frames       - number of frames per second
//    Return:   void
//
//    FIXME: aligning the movement when another key is pressed at the same
//    time.
//
//==========================================================================

void ProtagMovement(bool keys[], float *px, float *py, float sp, int *fx, int *fy,
                    float frames)
{
    int dx = 0, dy = 0;  // current direction

    MoveInput(keys, &dx, &dy, fx, fy, frames);

    // apply speed
    float mov_x = dx * sp;
    float mov_y = dy * sp;
    // adjust speed
    if (dx != 0 && dy != 0) {
        float adj = 0.707f;  // aka 1 / sqrt(2)
        mov_x *= adj;
        mov_y *= adj;
    }

    CollVSMove(px, py, mov_x, mov_y);
}

//==========================================================================
//
//    SpriteAimAtCursor
//
//    Argument: float px, py    - actor's X and Y position
//              float *fy       - sprite sheet's column to animate
//    Return:   void
//
//==========================================================================

void SpriteAimAtCursor(float px, float py, int *fy)
{
    float t_dx    = mouse_x - (px + 16);  // sprite center
    float t_dy    = mouse_y - (py + 24);  // same above
    float t_angle = atan2(t_dy, t_dx);    // radianus (-PI to +PI)

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
