//**************************************************************************
//**
//** File: main.c (CyberSP Project)
//** Purpose: Main game stuff
//**
//** Last Update: 03-10-2025 15:37
//** Author: DDeyTS
//**
//**************************************************************************

/*
 * LIST OF FEATURES TO DO (17-09-25)
 * 1. Collision walls on the map. (Done!)
 *     1a. Map changer.
 * 2. NPC sprite render. (Done but it call for updates)
 *     2a. Entity movement manager. (Done!)
 * 3. Shooter mode. (Done!)
 *     3a. Shooting with pistol. (Done!)
 *     3b. Damage system. (Partially done!)
 * 4. Simple inventory (like first Metal Gear).
 * 5. Mouth cursor.
 * 6. Game state system.
 */

#include "main.h"
#include "bitmap.h"
#include "collision.h"
#include "debug.h"
#include "dialoguesys.h"
#include "dice.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include "textdat.h"
#include "tile_render.h"

#include <tmx.h>

// EXTERNAL FUNCTION PROTOTYPES /////////////////////////////////////////////

// PRIVATE FUNCTION PROTOTYPES //////////////////////////////////////////////

static void InitGame(void);

// EXTERNAL DATA DECLARATIONS ///////////////////////////////////////////////

tmx_map *map = NULL;

ALLEGRO_DISPLAY      *disp;
ALLEGRO_EVENT         ev;
ALLEGRO_EVENT_QUEUE  *queue;
ALLEGRO_TIMER        *timer;
ALLEGRO_AUDIO_STREAM *bgm, *dlg_ost, *tense_ost;

float dt = 1.0 / 30;

bool keys[ALLEGRO_KEY_MAX], mouse[MOUSE_MAX + 1];
int  mouse_x, mouse_y = 0;

int selected_topic = 0;
int active_topic   = -1;

bool show_desc = false;

bool running, redraw;

// PUBLIC DATA DEFINITIONS //////////////////////////////////////////////////

// PRIVATE DATA DEFINITIONS /////////////////////////////////////////////////

//==========================================================================
//
//    main
//    Arguments: void
//    Return:    int
//
//==========================================================================

int main(void)
{
    //
    // Initializers
    //

    InitGame();

    //
    // Event Queue
    //

    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_start_timer(timer);

    //
    // Main Loop
    //

    // reset these arrays
    memset(keys, 0, sizeof(keys));
    memset(mouse, 0, sizeof(mouse));

    // defines callbacks for libTMX
    tmx_img_load_func = AllegTexLoader;
    tmx_img_free_func = AllegTexFree;
    map               = tmx_load("tiles/avenue_cyber.tmx");
    if (!map) {
        tmx_perror("Cannot load map");
        return 1;
    }

    running = true;
    redraw  = true;

    while (running) {
        GameLoop();
    }

    GameCrusher();
    return 0;
}

void InitGame(void)
{

    if (!al_init() || !al_init_image_addon() || !al_init_primitives_addon() ||
        !al_install_keyboard() || !al_init_font_addon() || !al_init_ttf_addon() ||
        !al_install_mouse() || !al_install_audio() || !al_init_acodec_addon() ||
        !al_reserve_samples(16)) {
        perror("Fail to initialize Allegro\n");
        exit(1);
    }

    disp      = al_create_display(DISPW, DISPH);
    queue     = al_create_event_queue();
    timer     = al_create_timer(dt);
    bgm       = LoadMusic("soundtrack/cybersp_menu.ogg");
    dlg_ost   = LoadMusic("soundtrack/slow_talk.ogg");
    tense_ost = LoadMusic("soundtrack/tense_talk.ogg");
    if (!disp || !queue || !timer || !bgm) {
        perror("Fail to initialize basic Allegro stuff!\n");
        exit(1);
    }
    srand(time(NULL));

    InitStdFont();
    InitBitmap();
    InitCursor(disp);
    NpcDlgStorage(npc);
    DescStorage();
    SpawnAllEnemies(RollD4());  // NOTE: use num_active_enemies for a solid group.
    PlayMusic(bgm);
}
