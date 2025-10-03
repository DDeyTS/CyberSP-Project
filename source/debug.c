//**************************************************************************
//**
//** File: debug.c (CyberSP Project)
//** Purpose: Debugging stuff
//** Last Update: 03-10-2025 15:37
//** Author: DDeyTS
//**
//**************************************************************************

#include "debug.h"
#include "dialoguesys.h"
#include "dice.h"
#include "enemy.h"
#include "main.h"
#include "sound.h"

//==========================================================================
//
//    DBG_DescObjChanger
//
//    Argument: void
//    Return:   void
//
//==========================================================================

void DBG_DescObjChanger(void)
{
    obj_desc++;
    if (obj_desc > 2) {
        perror("Error to update object description!\n");
        exit(1);
    }
}

//==========================================================================
//
//    DBG_DlgTrigger
//
//    Argument: void
//    Return:   void
//
//==========================================================================

void DBG_DlgTrigger(void)
{
    dlgstats.flags = SHOW_INTRO | DLG_OPEN | CHOOSING_TOPIC;
}

//==========================================================================
//
//    DBG_NpcChanger
//
//    Argument: void
//    Return:   void
//
//==========================================================================

void DBG_NpcChanger(void)
{
    if (dlgstats.speaker < NUM_NPCS) {
        dlgstats.speaker++;
        printf("Hello, NPC number %d!\n", dlgstats.speaker);
        if (dlgstats.speaker == NPC_BANGER_THE_KILLER) {
            if (dlg_ost)
                PauseMusic(dlg_ost);  // FIXME: they aren't pausing
            else if (bgm)
                PauseMusic(bgm);
            PlayMusic(tense_ost);
        }
    }
}

//==========================================================================
//
//    DBG_EnemySpawner
//
//    Argument: void
//    Return:   void
//
//==========================================================================

void DBG_EnemySpawner(void)
{
    if (num_active_enemies < MAX_ENEMIES) {
        int num_active_enemies = RollD4();
        SpawnAllEnemies(num_active_enemies);
        printf("%d inimigos spawnados\n", num_active_enemies);
    }
}
