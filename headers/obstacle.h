#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "objet.h"

/* MACRO etat */


typedef struct {
    objet o;
    /* plus ? */
} obstacle;

obstacle creer_obstacle();
void liberer_obstacle(obstacle ob);

#endif /* _OBSTACLE_H_ */