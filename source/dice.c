//**************************************************************************
//**
//** File: dice.c 
//** Purpose: Random number handling
//** Last Update: 16-09-25 14:18
//** Author: DDeyTS
//**
//**************************************************************************

#include "game.h"
#include "input.h"

// EXTERNAL FUNCTION PROTOTYPES ////////////////////////////////////////////

// PUBLIC FUNCTION PROTOYPES ///////////////////////////////////////////////

// PRIVATE FUNCTION PROTOTYPES /////////////////////////////////////////////

// EXTERNAL DATA DECLARATIONS //////////////////////////////////////////////

// PUBLIC DATA DEFINITIONS /////////////////////////////////////////////////

// PRIVATE DATA DEFINITIONS ////////////////////////////////////////////////

//==========================================================================
//
//    RollD6
//
//    Argument: void 
//    Return:   int
//
//    TODO: Critical damage function.
//
//==========================================================================

int RollD6(void) { return (rand() % 6) + 1; }

