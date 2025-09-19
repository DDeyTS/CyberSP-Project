#pragma once

#include "game.h"
#include "input.h"
#include "main.h"

static inline int RollD6(void) { return (rand() % 6) + 1; }

static inline int RollD20(void) { return (rand() % 20) + 1; }
