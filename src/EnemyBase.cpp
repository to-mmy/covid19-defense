#include "enemyBase.h"

void EnemyBase::lowerHealth() {
    if (health > 0) {
        health--;
    }
    if (health <= 0) {
        isAlive = false;
    }
}
