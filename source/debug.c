//**************************************************************************
//**
//** File: debug.c (CyberSP Project)
//** Purpose: Debugging stuff
//** Last Update: 25-08-2025 20:45
//** Author: DDeyTS
//**
//**************************************************************************

#include "debug.h"
#include "dialoguesys.h"
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
    dlg_open       = true;
    choosing_topic = true;
    show_intro     = true;
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
    speaker++;
    if (speaker >= NUM_NPCS) {
        perror("You has exceed the NPC limit.\n");
        exit(1);
    }
}
