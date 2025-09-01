//**************************************************************************
//**
//** File: game.c (CyberSP Project)
//** Purpose: Game logic
//** Last Update: 01-09-2025 12:29
//** Author: DDeyTS
//**
//**************************************************************************

#include "game.h"
#include "bitmap.h"
#include "dialoguesys.h"
#include "input.h"
#include "main.h"
#include "tile_render.h"

//==========================================================================
//
//    GameRun
//
//    Argument: void
//    Return:   void
//
//    NOTE: this function is responsible to redraw bitmaps.
//
//==========================================================================

void GameRun(void)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    RenderMap(map);

    DrawProtag();
    DrawEntity();

    // NOTE: rectangle to debug description window
    // al_draw_filled_rectangle(350, 125, 450, 200, al_map_rgba(0, 100, 0,
    // 200));
    if (show_desc) {
        InitDescBox(desc[obj_desc]->pos_x, desc[obj_desc]->pos_y,
                    desc[obj_desc]->text);
    }

    if (dlg_open) {
        if (show_intro) {
            InitDlgBox(npc[speaker]->portrait_id, npc[speaker]->name,
                       npc[speaker]->topics->intro_text);
        }
        else if (active_topic >= 0) {
            const char *topic = npc[speaker]->topics[selected_topic].topic;
            LoadDlg(npc[speaker], topic);
        }
        InitTopicMenu(npc[speaker], selected_topic);
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

        //
        // character animation
        //

        frames += 0.3f;    // frame speed
        if (frames > 4) {  // reset frame queue
            frames -= 4;
        }

        ToggleToAim();

        redraw = true;
    }

    if (redraw && al_is_event_queue_empty(queue)) {
        GameRun();
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
    // Dialogue Sys
    al_destroy_bitmap(npc[speaker]->portrait_id);
    for (int i = 0; i < npc[speaker]->num_topic; i++) {
        free(npc[speaker]->topics[i].topic);
        free(npc[speaker]->topics[i].text);
    }
    free(npc[speaker]->topics);
    free(npc[speaker]);

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
