//**************************************************************************
//**
//** File: debug.c (CyberSP Project)
//** Purpose: Debugging stuff
//** Last Update: 19-09-2025 13:17
//** Author: DDeyTS
//**
//**************************************************************************

#include "debug.h"
#include "dialoguesys.h"
#include "dice.h"
#include "enemy.h"
#include "main.h"

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
    }
}

void DBG_EnemySpawner(void)
{
    if (num_active_enemies < MAX_ENEMIES) {
        int roll = RollD4();
        SpawnAllEnemies(roll);
        printf("%d inimigos spawnados\n", roll);
    }
}
