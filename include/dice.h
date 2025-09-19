#pragma once

#include "game.h"
#include "input.h"
#include "main.h"

static inline int RollD6(void) { return (rand() % 6) + 1; }
static inline int RollD20(void) { return (rand() % 20) + 1; }
static inline int RollD8(void) { return (rand() % 8) + 1; }
static inline int RollD4(void) { return (rand() % 4) + 1; }
