//**************************************************************************
//**
//** File: game.c (CyberSP Project)
//** Purpose: Game logic
//** Last Update: 02-09-2025 23:57
//** Author: DDeyTS
//**
//**************************************************************************

#include "game.h"
#include "bitmap.h"
#include "dialoguesys.h"
#include "input.h"
#include "main.h"
#include "tile_render.h"
#include <stdlib.h>

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

    if (dlgstats.dlg_open) {
        if (dlgstats.show_intro) {
            InitDlgBox(npc[dlgstats.speaker]->portrait_id,
                       npc[dlgstats.speaker]->name,
                       npc[dlgstats.speaker]->topics->intro_text);
        }
        else if (active_topic >= 0) {
            const char *topic = npc[dlgstats.speaker]->topics[selected_topic].topic;
            LoadDlg(npc[dlgstats.speaker], topic);
        }
        InitTopicMenu(npc[dlgstats.speaker], selected_topic);
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
        // protagonist's walking animation
        //
        {
            protag.frames += 0.3f;    // frame speed
            if (protag.frames > 4) {  // reset frame queue
                protag.frames -= 4;
            }
            if (chosen_cursor == cursors.aim) {
                ToggleToAim();
            }
            else {
                ProtagMovement(keys, &protag.px, &protag.py, protag.speed,
                               &protag.frame_w, &protag.frame_h,
                               (int)protag.frames);
            }
        }

        //
        // NPC random movement
        //

        // TODO: upgrade that into a function
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
            EntityMovement(e, &ent[e].px, &ent[e].py, ent[e].speed, &ent[e].frame_w,
                           &ent[e].frame_h, (int)ent[e].frames, npc_x, npc_y);
        }

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
    //
    // Dialogue Sys
    //

    al_destroy_bitmap(npc[dlgstats.speaker]->portrait_id);
    for (int i = 0; i < npc[dlgstats.speaker]->num_topic; i++) {
        free(npc[dlgstats.speaker]->topics[i].topic);
        free(npc[dlgstats.speaker]->topics[i].text);
    }
    free(npc[dlgstats.speaker]->topics);
    free(npc[dlgstats.speaker]);

    // 
    // Tile Mapping 
    //

    tmx_map_free(map);

    // 
    // Fonts 
    //

    ExplodeFont();

    // 
    // Sprites 
    //

    BitmapExplode();

    // 
    // Cursors 
    //

    if (cursors.normal) al_destroy_mouse_cursor(cursors.normal);
    if (cursors.view) al_destroy_mouse_cursor(cursors.view);
    if (cursors.aim) al_destroy_mouse_cursor(cursors.aim);
    if (cursors.clicking) al_destroy_mouse_cursor(cursors.clicking);

    // 
    // Allegro Stuff 
    //

    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
    al_destroy_display(disp);
}
