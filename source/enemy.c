

#include "enemy.h"
#include "bitmap.h"
#include "game.h"

SpawnEnemyData_t spawndata[NUM_ENEMY] = {
{EN_GANGMEMBER, 100, 240}, 
{EN_GANGMEMBER2, 200, 300}
};

//
//===========================
//
// InitEnemy
//
//===========================
//

void InitEnemy(int id, float start_x, float start_y)
{
    if (id < 0 || id >= NUM_ENEMY) return;
        en[id].px = start_x;
        en[id].py = start_y;
}

//
//===========================
//
// SpawnAllEnemies
//
//===========================
//

void SpawnAllEnemies(void)
{
    for (int i = 0; i < NUM_ENEMY; i++) {
        InitEnemy(spawndata[i].id, spawndata[i].x, spawndata[i].y);
    }
}
