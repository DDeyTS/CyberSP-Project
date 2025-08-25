
#include "debug.h"
#include "dialoguesys.h"
#include "main.h"

void DBG_DescObjChanger(void)
{
    obj_desc++;
    if (obj_desc > 2) {
        perror("Error to update object description!\n");
        exit(1);
    }
}

void DBG_DlgTrigger(void)
{
    dlg_open       = true;
    choosing_topic = true;
    show_intro     = true;
}

void DBG_NpcChanger(void)
{
    speaker++;
    if (speaker >= NUM_NPCS) {
        perror("You has exceed the NPC limit.\n");
        exit(1);
    }
}
