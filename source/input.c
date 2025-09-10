//**************************************************************************
//**
//** File: input.c (CyberSP Project)
//** Purpose: Organize all input stuff
//** Last Update: 08-09-2025 13:59
//** Author: DDeyTS
//**
//**************************************************************************

#include "input.h"
#include "bitmap.h"
#include "debug.h"
#include "dialoguesys.h"
#include "main.h"

// EXTERNAL DATA DECLARATIONS //////////////////////////////////////////////

Mousecursors cursors;
ALLEGRO_MOUSE_CURSOR *current_cursor = NULL;
enum CursorType cursor_flag          = CURSOR_NORMAL;
ALLEGRO_MOUSE_CURSOR *chosen_cursor;
bool mouse_animating = false;

// PRIVATE DATA DEFINITIONS ////////////////////////////////////////////////

static double anim_timer    = 0.0;   // NOTE: in case of trouble, use double
static double anim_duration = 0.15;  // NOTE: same above

//==========================================================================
//
//    KeyboardOn
//
//    Argument: void
//    Return:   void
//
//    NOTE: this functions contains every keyboard operation.
//
//==========================================================================

void KeyboardOn(void)
{
    //
    // Keyboard Boolean
    //

    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        keys[ev.keyboard.keycode] = true;
    }
    else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
        keys[ev.keyboard.keycode] = false;
    }

    //
    // Cursor Bitmap Changer
    //

    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        CursorChanger();
    }

    //
    // Object Description Changer
    //

    if (ev.type == ALLEGRO_EVENT_KEY_DOWN && keys[ALLEGRO_KEY_O]) {
        DBG_DescObjChanger();
    }

    //
    // Dialogue Trigger Button
    //

    if (ev.type == ALLEGRO_EVENT_KEY_DOWN && keys[ALLEGRO_KEY_SPACE]) {
        DBG_DlgTrigger();
    }

    //
    // Dialogue Box Exit
    //

    // TODO: bitmap button to trigger it
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN && keys[ALLEGRO_KEY_ESCAPE]) {
        DlgExit();
    }

    //
    // NPC Changer
    //

    if (ev.type == ALLEGRO_EVENT_KEY_DOWN && keys[ALLEGRO_KEY_1]) {
        DBG_NpcChanger();
    }
    else if (ev.type == ALLEGRO_EVENT_KEY_DOWN && keys[ALLEGRO_KEY_2]) {
        dlgstats.speaker = 0;
    }
}

//==========================================================================
//
//    MouseOn
//
//    Argument: void
//    Return:   void
//
//    NOTE: this function contains every mouse operation.
//
//==========================================================================

void MouseOn(void)
{
    //
    // Global Mouse Position
    //

    if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
        mouse_x = ev.mouse.x;
        mouse_y = ev.mouse.y;
    }

    //
    // Mouse Boolean
    //

    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        mouse[ev.mouse.button] = true;

        // checks if eye cursor click on the object
        if (chosen_cursor == cursors.view && mouse[1]) {
            int rect_x = desc[obj_desc]->pos_x, rect_y = desc[obj_desc]->pos_y;
            int rect_w = rect_x + 100, rect_h = rect_y + 75;

            // finds the collisin to trigger InitDescBox()
            if (mouse_x >= rect_x && mouse_x <= rect_w && mouse_y >= rect_y &&
                mouse_y <= rect_h) {
                show_desc = true;
                // debugger below
                printf("Apareceu a caixa!\n");
            }
        }
    }
    else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        mouse[ev.mouse.button] = false;
    }

    //
    // Click Animation
    //

    if (mouse[1] && chosen_cursor == cursors.normal) {
        mouse_animating = true;
        anim_timer      = al_get_time();
        if (cursors.clicking) {
            current_cursor = cursors.clicking;
            al_set_mouse_cursor(disp, current_cursor);
        }
    }

    //
    // Dialogue Interaction
    //

    bool dlg_open       = (dlgstats.flags & DLG_OPEN) == DLG_OPEN;
    bool choosing_topic = (dlgstats.flags & CHOOSING_TOPIC) == CHOOSING_TOPIC;
    if ((ev.type == ALLEGRO_EVENT_MOUSE_AXES ||
         ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) &&
        dlg_open) {

        if (!choosing_topic && ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            // nothing happens, just ignore the event
        }
        else {
            mouse_x = ev.mouse.x;
            mouse_y = ev.mouse.y;

            int tx      = 50;  // topics' axes
            int ty      = 350;
            int spacing = 20;   // vertical space between topics
            int topic_w = 150;  // area able to click on
            int topic_h = spacing;

            for (int i = 0; i < npc[dlgstats.speaker]->num_topic; i++) {
                int top_y = ty + i * spacing;

                // checks if mouse is inside the clickable area of the topic
                if (mouse_x >= tx && mouse_x <= tx + topic_w && mouse_y >= top_y &&
                    mouse_y <= top_y + topic_h) {
                    selected_topic = i;  // defines the chosen topic

                    if (!choosing_topic) {
                        dlgstats.flags |= CHOOSING_TOPIC;
                        active_topic = -1;  // no active topic, yet
                    }

                    if (mouse[1]) {
                        active_topic = selected_topic;  // topic is activated
                        dlgstats.flags &= ~(SHOW_INTRO | CHOOSING_TOPIC);
                    }

                    break;  // finish loop when the mouse finds a topic
                }
            }
        }
    }
}

//==========================================================================
//
//    MouseClick
//
//    Argument: void
//    Return:   void
//
//==========================================================================

void MouseClick(void)
{
    if (mouse_animating) {
        if (current_cursor != cursors.clicking && cursors.clicking) {
            current_cursor = cursors.clicking;
            al_set_mouse_cursor(disp, current_cursor);
        }
        if ((al_get_time() - anim_timer) >= anim_duration) {
            mouse_animating = false;

            current_cursor = chosen_cursor;
            al_set_mouse_cursor(disp, current_cursor);
        }
    }
    else {
        if (current_cursor != cursors.normal) {
            current_cursor = chosen_cursor;
            al_set_mouse_cursor(disp, current_cursor);
        }
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
        !cursors.target_bmp) {
        perror("Fail to load cursor bitmap\n");
        return false;
    }

    cursors.normal   = al_create_mouse_cursor(cursors.mouse_bmp, 0, 0);
    cursors.clicking = al_create_mouse_cursor(cursors.click_bmp, 0, 0);
    cursors.aim      = al_create_mouse_cursor(cursors.target_bmp, 0, 0);
    cursors.view     = al_create_mouse_cursor(cursors.eye_bmp, 0, 0);
    if (!cursors.normal || !cursors.clicking || !cursors.aim || !cursors.view) {
        fprintf(stderr, "Error: fail to to create mouse cursor!\n");
        return false;
    }

    chosen_cursor  = cursors.normal;
    cursor_flag    = CURSOR_NORMAL;
    current_cursor = chosen_cursor;
    al_set_mouse_cursor(disp, current_cursor);
    al_show_mouse_cursor(disp);

    return true;
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
    bool dlg_open = (dlgstats.flags & DLG_OPEN) == DLG_OPEN;

    if (keys[ALLEGRO_KEY_T]) {          // NOTE: target mode
        chosen_cursor = cursors.aim;    // changes the cursor bitmap
        cursor_flag   = CURSOR_TARGET;  // flags the current cursor

        if (dlg_open)
            dlgstats.flags &= ~DLG_OPEN;  // doesn't work during dialogue window

        if (!mouse_animating) {  // doesn't work during cursor click
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

        if (dlg_open) dlgstats.flags &= ~DLG_OPEN;

        if (!mouse_animating) {
            current_cursor = chosen_cursor;
            al_set_mouse_cursor(disp, current_cursor);
        }
    }
}

//==========================================================================
//
//    CloseGame
//
//    Argument: void
//    Return:   void
//
//==========================================================================

void CloseGame(void)
{
    if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) running = false;
}

//==========================================================================
//
//    ToggleToAim
//
//    Argument: void
//    Return:   void
//
//==========================================================================

void ToggleToAim(void)
{
    // sprite follows cursor when aiming
    SpriteAimAtCursor(protag.px, protag.py, &protag.fh);
    protag.fw = 0;
}
