#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "matrice.h"

// AABB, Amélioration : OBB avec SAT

int collisions_AABB(matrice hitbox1, matrice hitbox2);
int collisions_OBB(matrice hitboxA, matrice hitboxB);

#endif /* _COLLISION_H_ */