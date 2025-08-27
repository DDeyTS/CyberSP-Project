//**************************************************************************
//**
//** File: main.c (CyberSP Project)
//** Purpose: Main game stuff
//**
//** Last Update: 25-08-2025 15:44
//** Author: DDeyTS
//**
//**************************************************************************

/*
 * LIST OF FEATURES TO DO (27-08-25)
 * 1. Collision walls on the map.
 * 2. NPC sprite render.
 * 3. Shooter mode.
 *     3a. Shooting with pistol.
 *     3b. Damage system.
 */

#include "main.h"
#include "bitmap.h"
#include "collision.h"
#include "debug.h"
#include "dialoguesys.h"
#include "game.h"
#include "input.h"
#include "textdat.h"
#include "tile_render.h"

#include <tmx.h>

// EXTERNAL FUNCTION PROTOTYPES /////////////////////////////////////////////

// PRIVATE FUNCTION PROTOTYPES //////////////////////////////////////////////

// EXTERNAL DATA DECLARATIONS ///////////////////////////////////////////////

tmx_map *map = NULL;

ALLEGRO_DISPLAY *disp;
ALLEGRO_EVENT ev;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_TIMER *timer;

// keyboard stuff
bool keys[ALLEGRO_KEY_MAX], mouse[MOUSE_MAX + 1];
int mouse_x, mouse_y = 0;

// dialogue sys stuff
bool show_intro     = true;  // turn on the intro dialogue
bool dlg_open       = false; // turn on the chat mode
bool choosing_topic = true;  // turn on the topic list
int speaker         = 0;     // flags the current NPC
int selected_topic  = 0;
int active_topic    = -1;

// description window stuff
int obj_desc   = 0; // debugger to change the description texts
bool show_desc = false;

// sprite stuff
float sp;
float frames;

// loop stuff
bool running, redraw;

// PUBLIC DATA DEFINITIONS //////////////////////////////////////////////////

// PRIVATE DATA DEFINITIONS /////////////////////////////////////////////////

//==========================================================================
//
//    Main Game
//
//    Initializers, game loop, crusher... etc.
//
//==========================================================================

int main(void)
{
    // INITIALIZERS /////////////////////////////////////////////////////////

    if (!al_init() || !al_init_image_addon() || !al_init_primitives_addon() ||
        !al_install_keyboard() || !al_init_font_addon() || !al_init_ttf_addon() ||
        !al_install_mouse()) {
        perror("Fail to initialize Allegro\n");
        return 1;
    }

    disp  = al_create_display(DISPW, DISPH);
    queue = al_create_event_queue();
    timer = al_create_timer(1.0 / 30.0);
    if (!disp || !queue || !timer) {
        perror("Fail to initialize basic Allegro stuff!\n");
        return 1;
    }

    InitStdFont();
    InitBitmap();
    InitCursor(disp);
    NpcDlgStorage(npc);
    DescStorage(desc);

    // EVENT QUEUE //////////////////////////////////////////////////////////

    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_start_timer(timer);

    // PLAYER MOVEMENT //////////////////////////////////////////////////////

    spr.px = 320, spr.py = 200;
    spr.frame_w = 0, spr.frame_h = 0;
    sp = 3.5; // movement speed
    frames = 0.f;

    // LOOP //////////////////////////////////////////////////////////////// 

    // reset these arrays
    memset(keys, 0, sizeof(keys));
    memset(mouse, 0, sizeof(mouse));

    // defines callbacks for libTMX
    tmx_img_load_func = AllegTexLoader;
    tmx_img_free_func = AllegTexFree;
    map               = tmx_load("tiles/buspoint.tmx");
    if (!map) {
        tmx_perror("Cannot load map");
        return 1;
    }

    running = true;
    redraw  = true;
    while (running) {GameLoop();}

    GameCrusher();
    return 0;
}
