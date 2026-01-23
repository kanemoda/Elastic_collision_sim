#ifndef PHYSICS_H
#define PHYSICS_H

void Physics_HandleWallCollisions(void);
void Physics_HandleParticleCollisionsNaive(void);
void Physics_HandleParticleCollisionsGrid(void);
void Physics_HandleParticleCollisionsGrid_PBD(void);

#endif
