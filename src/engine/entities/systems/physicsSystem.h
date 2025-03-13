#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "../entity.h"
#include "../tiles.h"

void updatePhysicEntity(t_entity* entity, float* deltaTime, t_grid* grid, t_objectManager* entities);
void resolveCollision(t_entity* t_entity, t_grid* grid, t_objectManager* entities);

#endif