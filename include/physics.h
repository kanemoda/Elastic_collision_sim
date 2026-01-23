#ifndef PHYSICS_H
#define PHYSICS_H

extern long long g_pair_checks;
extern long long g_overlap_pass;
extern long long g_impulse_tests;
extern long long g_sqrt_calls;
extern long long g_impulses_applied;

void Physics_HandleWallCollisions(void);
void Physics_HandleParticleCollisionsNaive(void);
void Physics_HandleParticleCollisionsGrid(void);

#endif
